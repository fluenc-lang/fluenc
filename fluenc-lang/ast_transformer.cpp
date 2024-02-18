#include <map>
#include <numeric>
#include <optional>
#include <unordered_set>
#include <vector>

#include <fmt/core.h>

#include "peglib.h"

#include "ast.hpp"
#include "ast_transformer.hpp"
#include "indexed.hpp"
#include "namespace.hpp"
#include "utility.hpp"

#include "iterators/index_iterator.hpp"

#include "ast/argument_node.hpp"
#include "ast/function_node.hpp"
#include "ast/module_node.hpp"
#include "ast/type_name_node.hpp"

namespace fluenc
{
	ast_transformer::ast_transformer(
		const std::vector<std::string>& namespaces,
		const std::optional<uint64_t>& iterator,
		const std::optional<expression_t>& parent,
		const std::optional<expression_t>& alpha,
		const std::optional<expression_t>& beta
	)
		: namespaces_(namespaces)
		, iterator_(iterator)
		, parent_(parent)
		, alpha_(alpha)
		, beta_(beta)
	{
	}

	std::vector<std::string> qualified_names(const std::vector<std::string>& namespaces, const std::string& name)
	{
		if (name.rfind("::") == 0)
		{
			return { name.substr(2) };
		}

		std::ostringstream qualified_name;

		std::copy(begin(namespaces), end(namespaces), std::ostream_iterator<std::string>(qualified_name, "::"));

		qualified_name << name;

		if (qualified_name.str() == name)
		{
			return { name };
		}

		return { qualified_name.str(), name };
	}

	void populate_instructions(
		const std::vector<std::string>& namespaces,
		const std::vector<std::any>& instructions,
		std::vector<const function_node*>& roots,
		std::multimap<std::string, function_node*>& functions,
		std::map<std::string, const code_generation::base_value*>& locals,
		std::map<std::string, expression_t>& globals,
		std::map<std::string, struct_node*>& types
	)
	{
		for (auto& instruction : instructions)
		{
			if (instruction.type() == typeid(namespace_t*))
			{
				auto _namespace = std::any_cast<namespace_t*>(instruction);

				std::vector<std::string> nested_namespaces(namespaces);

				nested_namespaces.push_back(_namespace->name);

				populate_instructions(nested_namespaces, _namespace->children, roots, functions, locals, globals, types);
			}
			else
			{
				if (instruction.type() == typeid(function_node*))
				{
					auto callable = std::any_cast<function_node*>(instruction);

					if (callable->type == function_type::exported)
					{
						roots.push_back(callable);

						auto import = new function_node {
							.signature = callable->signature,
							.type = function_type::imported,
							.ast = callable->ast,
							.block = {},
						};

						auto name = qualified_names(namespaces, callable->signature.name);

						functions.insert({ name[0], import });
					}
					else
					{
						auto name = qualified_names(namespaces, callable->signature.name);

						functions.emplace(name[0], callable);
					}
				}
				else if (instruction.type() == typeid(struct_node*))
				{
					auto prototype = std::any_cast<struct_node*>(instruction);

					auto name = qualified_names(namespaces, prototype->tag);

					types.insert({ name[0], prototype });
				}
				else if (instruction.type() == typeid(global_node*))
				{
					auto global = std::any_cast<global_node*>(instruction);

					auto name = qualified_names(namespaces, global->name);

					globals.insert({ name[0], global });
				}
			}
		}
	}

	module_node ast_transformer::transform(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<const function_node*> roots;
		std::multimap<std::string, function_node*> functions;
		std::map<std::string, const code_generation::base_value*> locals;
		std::map<std::string, expression_t> globals;
		std::map<std::string, struct_node*> types;

		std::vector<std::any> results;

		std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(results), [this](auto instruction) {
			return visit_instruction(instruction);
		});

		populate_instructions(std::vector<std::string>(), results, roots, functions, locals, globals, types);

		return module_node {
			roots, functions, locals, globals, types,
		};
	}

	std::any ast_transformer::visit_instruction(const std::shared_ptr<peg::Ast>& ast) const
	{
		using namespace peg::udl;

		switch (ast->tag)
		{
		case "Instruction"_:
			return visit_instruction(ast->nodes[0]);
		case "Function"_:
			return visit_function(ast);
		case "Structure"_:
			return visit_structure(ast);
		case "Global"_:
			return visit_global(ast);
		case "Namespace"_:
			return visit_namespace(ast);
		case "Block"_:
			return visit_block(ast);
		}

		throw std::exception();
	}

	std::string ast_transformer::visit_id(const std::shared_ptr<peg::Ast>& ast) const
	{
		return ast->token_to_string();
	}

	std::string ast_transformer::visit_integer(const std::shared_ptr<peg::Ast>& ast) const
	{
		return ast->token_to_string();
	}

	std::string ast_transformer::visit_string(const std::shared_ptr<peg::Ast>& ast) const
	{
		static std::unordered_map<char, char> table = {
			{ 'n', '\n' }, { 'r', '\r' }, { 't', '\t' }, { '0', '\0' }, { '1', '\1' }, { '2', '\2' },
			{ '3', '\3' }, { '4', '\4' }, { '5', '\5' }, { '6', '\6' }, { '7', '\7' }, { '\\', '\\' },
		};

		auto input = ast->token_to_string();

		std::ostringstream stream;

		std::accumulate(begin(input), end(input), false, [&](auto substitute, auto token) {
			if (substitute)
			{
				if (auto iterator = table.find(token); iterator != end(table))
				{
					stream << iterator->second;
				}
			}
			else
			{
				if (token == '\\')
				{
					return true;
				}
				else
				{
					stream << token;
				}
			}

			return false;
		});

		return stream.str();
	}

	std::vector<type_name_t> ast_transformer::visit_type_list(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<type_name_t> results;

		std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(results), [this](auto node) {
			return visit_type_name(node);
		});

		return results;
	}

	std::vector<struct_field_t> ast_transformer::visit_field_list(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<struct_field_t> results;

		std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(results), [this](auto node) {
			return visit_field(node);
		});

		return results;
	}

	std::vector<std::string> ast_transformer::visit_id_list(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<std::string> results;

		std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(results), [this](auto node) {
			return visit_id(node);
		});

		return results;
	}

	expression_t ast_transformer::visit_expression(const std::shared_ptr<peg::Ast>& ast) const
	{
		using namespace peg::udl;

		switch (ast->tag)
		{
		case "Expression"_:
			return visit_expression(ast->nodes[0]);
		case "Literal"_:
			return visit_literal(ast);
		case "Binary"_:
			return visit_binary(ast);
		case "Unary"_:
			return visit_unary(ast);
		case "Member"_:
			return visit_member(ast);
		case "Call"_:
			return visit_call(ast);
		case "Instantiation"_:
			return visit_instantiation(ast);
		case "Conditional"_:
			return visit_conditional(ast);
		case "Array"_:
			return visit_array(ast);
		case "Tuple"_:
			return visit_tuple(ast);
		case "Group"_:
			return visit_group(ast);
		case "Expansion"_:
			return visit_expansion(ast);
		case "Local"_:
			return visit_local(ast);
		}

		throw std::exception();
	}

	expression_t ast_transformer::visit_literal(const std::shared_ptr<peg::Ast>& ast) const
	{
		using namespace peg::udl;

		switch (ast->tag)
		{
		case "Literal"_:
			return visit_literal(ast->nodes[0]);
		case "Int32Literal"_:
			return visit_int32_literal(ast);
		case "Int64Literal"_:
			return visit_int64_literal(ast);
		case "Float32Literal"_:
			return visit_float32_literal(ast);
		case "BooleanLiteral"_:
			return visit_boolean_literal(ast);
		case "StringLiteral"_:
			return visit_string_literal(ast);
		case "Uint32Literal"_:
			return visit_uint32_literal(ast);
		case "CharLiteral"_:
			return visit_char_literal(ast);
		case "ByteLiteral"_:
			return visit_byte_literal(ast);
		case "NothingLiteral"_:
			return visit_nothing_literal(ast);
		}

		throw std::exception();
	}

	expression_t ast_transformer::visit_int32_literal(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new integral_literal_node(*alpha_, default_type_name::int32(), visit_integer(ast->nodes[0]));
	}

	expression_t ast_transformer::visit_int64_literal(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new integral_literal_node(*alpha_, default_type_name::int64(), visit_integer(ast->nodes[0]));
	}

	expression_t ast_transformer::visit_float32_literal(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new float_literal_node(*alpha_, default_type_name::f32(), ast->token_to_string());
	}

	expression_t ast_transformer::visit_boolean_literal(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new boolean_literal_node(*alpha_, ast->token_to_string());
	}

	expression_t ast_transformer::visit_string_literal(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new string_literal_node(*alpha_, visit_string(ast->nodes[0]));
	}

	expression_t ast_transformer::visit_uint32_literal(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new integral_literal_node(*alpha_, default_type_name::uint32(), visit_integer(ast->nodes[0]));
	}

	expression_t ast_transformer::visit_char_literal(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new character_literal_node(*alpha_, visit_string(ast));
	}

	expression_t ast_transformer::visit_byte_literal(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new integral_literal_node(*alpha_, default_type_name::byte(), visit_integer(ast->nodes[0]));
	}

	expression_t ast_transformer::visit_nothing_literal(const std::shared_ptr<peg::Ast>& ast) const
	{
		UNUSED(ast);

		return new nothing_node(*alpha_);
	}

	expression_t ast_transformer::visit_binary(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto binary = new binary_node {
			.ast = ast,
			.consumer = *alpha_,
			.op = visit_id(ast->nodes[1]),
		};

		ast_transformer left_visitor(namespaces_, iterator_, parent_, binary, {});

		auto left = left_visitor.visit_expression(ast->nodes[0]);

		ast_transformer right_visitor(namespaces_, iterator_, parent_, left, {});

		auto right = right_visitor.visit_expression(ast->nodes[2]);

		return right;
	}

	expression_t ast_transformer::visit_member(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto access = visit_id_list(ast->nodes[0]);

		auto path = std::accumulate(begin(access) + 1, end(access), (*begin(access)), [](auto name, std::string node) {
			return fmt::format("{}.{}", name, node);
		});

		auto qualified_path = qualified_names(namespaces_, path);

		if (ast->nodes.size() > 1)
		{
			auto with = visit_with(ast->nodes[1]);

			return new member_access_node(with, ast, qualified_path);
		}

		return new member_access_node(*alpha_, ast, qualified_path);
	}

	expression_t ast_transformer::visit_call(const std::shared_ptr<peg::Ast>& ast) const
	{
		ast_transformer visitor(namespaces_, iterator_, parent_, terminator_node::instance(), {});

		std::vector<expression_t> values;

		if (ast->nodes[0]->token_to_string() == "tail")
		{
			std::transform(begin(ast->nodes) + 2, end(ast->nodes), std::back_inserter(values), [&](auto node) {
				return visitor.visit_expression(node);
			});

			auto names = qualified_names(namespaces_, visit_id(ast->nodes[1]));

			return new function_call_node(function_call_type::tail, ast, values, names, *alpha_);
		}
		else
		{
			std::transform(begin(ast->nodes) + 1, end(ast->nodes), std::back_inserter(values), [&](auto node) {
				return visitor.visit_expression(node);
			});

			auto names = qualified_names(namespaces_, visit_id(ast->nodes[0]));

			return new function_call_node(function_call_type::regular, ast, values, names, *alpha_);
		}
	}

	expression_t ast_transformer::visit_instantiation(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<std::string> fields;

		std::transform(begin(ast->nodes) + 1, end(ast->nodes), std::back_insert_iterator(fields), [this](auto assignment) {
			return visit_id(assignment->nodes[0]);
		});

		auto inst = new instantiation_node {
			.consumer = *alpha_,
			.prototype = default_prototype { .type = visit_type_name(ast->nodes[0]) },
			.ast = ast,
			.fields = fields,
		};

		return std::
			accumulate(begin(ast->nodes) + 1, end(ast->nodes), expression_t { inst }, [this](auto consumer, auto assignment) {
				ast_transformer visitor(namespaces_, iterator_, parent_, consumer, {});

				return visitor.visit_expression(assignment->nodes[1]);
			});
	}

	expression_t ast_transformer::visit_conditional(const std::shared_ptr<peg::Ast>& ast) const
	{
		ast_transformer block_visitor(namespaces_, iterator_, parent_, beta_, {});

		auto block = block_visitor.visit_block(ast->nodes[1]);

		auto conditional = new conditional_node(ast, block, *alpha_);

		ast_transformer expression_visitor(namespaces_, iterator_, parent_, conditional, {});

		auto condition = expression_visitor.visit_expression(ast->nodes[0]);

		return new block_instruction_node(condition, block->contains_iterator);
	}

	expression_t ast_transformer::visit_array(const std::shared_ptr<peg::Ast>& ast) const
	{
		if (ast->nodes.empty())
		{
			return new nothing_node(*alpha_);
		}

		if (ast->nodes.size() == 1)
		{
			return visit_expression(ast->nodes[0]);
		}

		std::vector<indexed<expression_t>> elements;

		std::transform(
			begin(ast->nodes),
			end(ast->nodes),
			index_iterator(0u),
			std::back_insert_iterator(elements),
			[&](auto x, auto y) -> fluenc::indexed<expression_t> {
				ast_transformer visitor(namespaces_, iterator_, parent_, terminator_node::instance(), {});

				return { y, visitor.visit_expression(x) };
			}
		);

		return new array_node(ast, elements, *alpha_);
	}

	expression_t ast_transformer::visit_tuple(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto sink = new tuple_sink_node(ast->nodes.size(), *alpha_);

		return std::accumulate(begin(ast->nodes), end(ast->nodes), expression_t { sink }, [&](auto consumer, auto value) {
			ast_transformer visitor(namespaces_, iterator_, parent_, consumer, {});

			return visitor.visit_expression(value);
		});
	}

	expression_t ast_transformer::visit_group(const std::shared_ptr<peg::Ast>& ast) const
	{
		return visit_expression(ast->nodes[0]);
	}

	expression_t ast_transformer::visit_expansion(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto expansion = new expansion_node(*alpha_, ast);

		ast_transformer visitor(namespaces_, iterator_, parent_, expansion, {});

		return visitor.visit_expression(ast->nodes[0]);
	}

	expression_t ast_transformer::visit_local(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto local = new local_node(*alpha_, visit_id(ast->nodes[0]));

		ast_transformer visitor(namespaces_, iterator_, parent_, local, {});

		return visitor.visit_expression(ast->nodes[1]);
	}

	expression_t ast_transformer::visit_continuation(const std::shared_ptr<peg::Ast>& ast) const
	{
		return std::accumulate(
			begin(ast->nodes) + 1,
			end(ast->nodes),
			expression_t { terminator_node::instance() },
			[this](const expression_t& consumer, auto parameter) {
				ast_transformer visitor(namespaces_, iterator_, parent_, consumer, {});

				return visitor.visit_expression(parameter);
			}
		);
	}

	expression_t ast_transformer::visit_with(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<std::string> fields;

		std::transform(begin(ast->nodes), end(ast->nodes), std::back_insert_iterator(fields), [this](auto assignment) {
			return visit_id(assignment->nodes[0]);
		});

		auto instantiation = new instantiation_node {
			.consumer = *alpha_,
			.prototype = with_prototype {},
			.ast = ast,
			.fields = fields,
		};

		return std::accumulate(
			begin(ast->nodes),
			end(ast->nodes),
			expression_t { instantiation },
			[this](auto consumer, auto assignment) {
				ast_transformer visitor(namespaces_, iterator_, parent_, consumer, {});

				return visitor.visit_expression(assignment->nodes[1]);
			}
		);
	}

	expression_t ast_transformer::visit_unary(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto unary = new unary_node {
			.ast = ast,
			.consumer = *alpha_,
			.op = visit_id(ast->nodes[0]),
		};

		ast_transformer visitor(namespaces_, iterator_, parent_, unary, {});

		return visitor.visit_expression(ast->nodes[1]);
	}

	function_node* ast_transformer::visit_function(const std::shared_ptr<peg::Ast>& ast) const
	{
		using namespace peg::udl;

		switch (ast->tag)
		{
		case "Function"_:
			return visit_function(ast->nodes[0]);
		case "RegularFunction"_:
			return visit_regular_function(ast);
		case "ImportedFunction"_:
			return visit_imported_function(ast);
		case "ExportedFunction"_:
			return visit_exported_function(ast);
		}

		throw std::exception();
	}

	function_node* ast_transformer::visit_regular_function(const std::shared_ptr<peg::Ast>& ast) const
	{
		static uint64_t iterator_id;

		auto name = visit_id(ast->nodes[0]);
		auto qualified_name = qualified_names(namespaces_, name);

		std::vector<argument_t> arguments;

		std::transform(begin(ast->nodes) + 1, end(ast->nodes) - 1, std::back_inserter(arguments), [this](auto argument) {
			return visit_argument(argument);
		});

		ast_transformer visitor(namespaces_, iterator_id++, parent_, terminator_node::instance(), {});

		return new function_node {
			.signature = {
				.arguments = arguments,
				.name = name,
			},
			.type = function_type::regular,
			.ast = ast,
			.block = visitor.visit_block(*ast->nodes.rbegin()),
		};
	}

	function_node* ast_transformer::visit_exported_function(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<argument_t> arguments;

		std::transform(begin(ast->nodes) + 2, end(ast->nodes) - 1, std::back_inserter(arguments), [this](auto argument) {
			return visit_argument(argument);
		});

		ast_transformer visitor(namespaces_, {}, parent_, terminator_node::instance(), {});

		return new function_node {
			.signature = {
				.return_type = visit_type_name(ast->nodes[0]),
				.arguments = arguments,
				.name = visit_id(ast->nodes[1]),
			},
			.type = function_type::exported,
			.ast = {},
			.block = visitor.visit_block(*ast->nodes.rbegin()),
		};
	}

	function_node* ast_transformer::visit_imported_function(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<argument_t> arguments;

		std::transform(begin(ast->nodes) + 2, end(ast->nodes), std::back_inserter(arguments), [this](auto argument) {
			return visit_argument(argument);
		});

		return new function_node {
			.signature = {
				.return_type = visit_type_name(ast->nodes[0]),
				.arguments = arguments,
				.name = visit_id(ast->nodes[1]),
			},
			.type = function_type::imported,
			.ast = ast,
			.block = {},
		};
	}

	argument_t ast_transformer::visit_argument(const std::shared_ptr<peg::Ast>& ast) const
	{
		using namespace peg::udl;

		switch (ast->tag)
		{
		case "Argument"_:
			return visit_argument(ast->nodes[0]);
		case "StandardArgument"_:
			return visit_standard_argument(ast);
		case "TupleArgument"_:
			return visit_tuple_argument(ast);
		}

		throw std::exception();
	}

	argument_t ast_transformer::visit_standard_argument(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto type = visit_type_name(ast->nodes[0]);
		auto name = visit_id(ast->nodes[1]);

		return new default_argument(name, type);
	}

	argument_t ast_transformer::visit_tuple_argument(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<argument_t> arguments;

		std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(arguments), [this](auto node) {
			return visit_argument(node);
		});

		return new tuple_argument(arguments);
	}

	struct_node* ast_transformer::visit_structure(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new struct_node {
			.ast = ast,
			.tag = visit_id(ast->nodes[0]),
			.fields = visit_field_list(ast->nodes[2]),
			.parent_types = visit_type_list(ast->nodes[1]),
		};
	}

	global_node* ast_transformer::visit_global(const std::shared_ptr<peg::Ast>& ast) const
	{
		ast_transformer visitor(namespaces_, iterator_, parent_, terminator_node::instance(), {});

		auto literal = visitor.visit_expression(ast->nodes[1]);

		return new global_node(ast, literal, visit_id(ast->nodes[0]));
	}

	namespace_t* ast_transformer::visit_namespace(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto name = ast->nodes[0]->token_to_string();
		auto namespaces = namespaces_;

		namespaces.push_back(name);

		ast_transformer visitor(namespaces, iterator_, parent_, alpha_, beta_);

		std::vector<std::any> children;

		std::transform(begin(ast->nodes) + 1, end(ast->nodes), std::back_inserter(children), [&](auto instruction) {
			return visitor.visit_instruction(instruction);
		});

		return new namespace_t(children, name);
	}

	block_instruction_node* ast_transformer::visit_return(const std::shared_ptr<peg::Ast>& ast) const
	{
		if (ast->nodes.size() > 1)
		{
			auto continuation = visit_continuation(ast->nodes[1]);

			auto ret = new return_node(iterator_, continuation, *alpha_);

			ast_transformer visitor(namespaces_, {}, parent_, ret, {});

			auto value = visitor.visit_expression(ast->nodes[0]);

			return new block_instruction_node(value, true);
		}

		auto ret = new return_node(iterator_, {}, *alpha_);

		ast_transformer visitor(namespaces_, {}, parent_, ret, {});

		auto value = visitor.visit_expression(ast->nodes[0]);

		return new block_instruction_node(value, false);
	}

	block_instruction_node* ast_transformer::visit_block(const std::shared_ptr<peg::Ast>& ast) const
	{
		ast_transformer visitor(namespaces_, iterator_, parent_, alpha_, beta_);

		auto first = rbegin(ast->nodes);

		auto ret = visitor.visit_return(*first);

		return std::accumulate(first + 1, rend(ast->nodes), ret, [&](auto consumer, auto expression) {
			ast_transformer visitor(namespaces_, iterator_, parent_, consumer, alpha_);

			auto value = visitor.visit_expression(expression);

			if (auto instruction = ast_cast<block_instruction_node>(value))
			{
				return new block_instruction_node(value, instruction->contains_iterator || consumer->contains_iterator);
			}

			return new block_instruction_node(value, consumer->contains_iterator);
		});
	}

	type_name_t ast_transformer::visit_type_name(const std::shared_ptr<peg::Ast>& ast) const
	{
		using namespace peg::udl;

		switch (ast->tag)
		{
		case "TypeName"_:
			return visit_type_name(ast->nodes[0]);
		case "RegularType"_:
			return visit_regular_type(ast);
		case "FunctionType"_:
			return visit_function_type(ast);
		}

		throw std::exception(); // TODO
	}

	type_name_t ast_transformer::visit_regular_type(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new default_type_name {
			.ast = ast,
			.names = qualified_names(namespaces_, visit_id(ast->nodes[0])),
		};
	}

	type_name_t ast_transformer::visit_function_type(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<type_name_t> types;

		std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(types), [this](auto typeName) {
			return visit_regular_type(typeName);
		});

		return new function_type_name {
			.types = types,
		};
	}

	struct_field_t ast_transformer::visit_field(const std::shared_ptr<peg::Ast>& ast) const
	{
		using namespace peg::udl;

		switch (ast->tag)
		{
		case "Field"_:
			return visit_field(ast->nodes[0]);
		case "StandardField"_:
			return visit_standard_field(ast);
		case "DecoratedField"_:
			return visit_decorated_field(ast);
		}

		throw std::exception();
	}

	struct_field_t ast_transformer::visit_standard_field(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto name = visit_id(ast->nodes[0]);

		if (ast->nodes.size() > 1)
		{
			ast_transformer visitor(namespaces_, iterator_, parent_, terminator_node::instance(), {});

			auto default_value = visitor.visit_expression(ast->nodes[1]);

			return { name, default_value, {} };
		}

		return { name, {}, {} };
	}

	struct_field_t ast_transformer::visit_decorated_field(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto type = visit_type_name(ast->nodes[0]);
		auto name = visit_id(ast->nodes[1]);

		if (ast->nodes.size() > 2)
		{
			ast_transformer visitor(namespaces_, iterator_, parent_, terminator_node::instance(), {});

			auto default_value = visitor.visit_expression(ast->nodes[2]);

			return { name, default_value, type };
		}

		return { name, {}, type };
	}
}
