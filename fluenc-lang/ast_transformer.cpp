#include <map>
#include <numeric>
#include <optional>
#include <unordered_set>
#include <vector>

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
		const iterator* iteratorType,
		const std::optional<expression_t>& parent,
		const std::optional<expression_t>& alpha,
		const std::optional<expression_t>& beta
	)
		: m_namespaces(namespaces)
		, m_iteratorType(iteratorType)
		, m_parent(parent)
		, m_alpha(alpha)
		, m_beta(beta)
	{
	}

	std::vector<std::string> qualifiedNames(const std::vector<std::string>& namespaces, const std::string& name)
	{
		if (name.rfind("::") == 0)
		{
			return { name.substr(2) };
		}

		std::ostringstream qualifiedName;

		std::copy(begin(namespaces), end(namespaces), std::ostream_iterator<std::string>(qualifiedName, "::"));

		qualifiedName << name;

		if (qualifiedName.str() == name)
		{
			return { name };
		}

		return { qualifiedName.str(), name };
	}

	void populateInstructions(
		const std::vector<std::string>& namespaces,
		const std::vector<std::any>& instructions,
		std::vector<const function_node*>& roots,
		std::multimap<std::string, function_node*>& functions,
		std::map<std::string, const BaseValue*>& locals,
		std::map<std::string, expression_t>& globals,
		std::map<std::string, struct_node*>& types,
		std::unordered_map<std::string, Use*>& uses
	)
	{
		for (auto& instruction : instructions)
		{
			if (instruction.type() == typeid(namespace_t*))
			{
				auto _namespace = std::any_cast<namespace_t*>(instruction);

				std::vector<std::string> nestedNamespaces(namespaces);

				nestedNamespaces.push_back(_namespace->name);

				populateInstructions(nestedNamespaces, _namespace->children, roots, functions, locals, globals, types, uses);
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
						};

						auto name = qualifiedNames(namespaces, callable->signature.name);

						functions.insert({ name[0], import });
					}
					else
					{
						auto name = qualifiedNames(namespaces, callable->signature.name);

						functions.emplace(name[0], callable);
					}
				}
				else if (instruction.type() == typeid(struct_node*))
				{
					auto prototype = std::any_cast<struct_node*>(instruction);

					auto name = qualifiedNames(namespaces, prototype->tag);

					types.insert({ name[0], prototype });
				}
				else if (instruction.type() == typeid(global_node*))
				{
					auto global = std::any_cast<global_node*>(instruction);

					auto name = qualifiedNames(namespaces, global->name);

					globals.insert({ name[0], global });
				}
			}
		}
	}

	module_node ast_transformer::transform(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<const function_node*> roots;
		std::multimap<std::string, function_node*> functions;
		std::map<std::string, const BaseValue*> locals;
		std::map<std::string, expression_t> globals;
		std::map<std::string, struct_node*> types;
		std::unordered_map<std::string, Use*> uses;

		std::vector<std::any> results;

		std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(results), [this](auto instruction) {
			return visitInstruction(instruction);
		});

		populateInstructions(std::vector<std::string>(), results, roots, functions, locals, globals, types, uses);

		return module_node {
			roots, functions, locals, globals, types, uses,
		};
	}

	std::any ast_transformer::visitInstruction(const std::shared_ptr<peg::Ast>& ast) const
	{
		using namespace peg::udl;

		switch (ast->tag)
		{
		case "Instruction"_:
			return visitInstruction(ast->nodes[0]);
		case "Function"_:
			return visitFunction(ast);
		case "Structure"_:
			return visitStructure(ast);
		case "Global"_:
			return visitGlobal(ast);
		case "Namespace"_:
			return visitNamespace(ast);
		case "Block"_:
			return visitBlock(ast);
		}

		throw std::exception();
	}

	std::string ast_transformer::visitId(const std::shared_ptr<peg::Ast>& ast) const
	{
		return ast->token_to_string();
	}

	std::string ast_transformer::visitInteger(const std::shared_ptr<peg::Ast>& ast) const
	{
		return ast->token_to_string();
	}

	std::string ast_transformer::visitString(const std::shared_ptr<peg::Ast>& ast) const
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

	std::vector<type_name_t> ast_transformer::visitTypeList(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<type_name_t> results;

		std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(results), [this](auto node) {
			return visitTypeName(node);
		});

		return results;
	}

	std::vector<struct_field_t> ast_transformer::visitFieldList(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<struct_field_t> results;

		std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(results), [this](auto node) {
			return visitField(node);
		});

		return results;
	}

	std::vector<std::string> ast_transformer::visitIdList(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<std::string> results;

		std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(results), [this](auto node) {
			return visitId(node);
		});

		return results;
	}

	expression_t ast_transformer::visitExpression(const std::shared_ptr<peg::Ast>& ast) const
	{
		using namespace peg::udl;

		switch (ast->tag)
		{
		case "Expression"_:
			return visitExpression(ast->nodes[0]);
		case "Literal"_:
			return visitLiteral(ast);
		case "Binary"_:
			return visitBinary(ast);
		case "Unary"_:
			return visitUnary(ast);
		case "Member"_:
			return visitMember(ast);
		case "Call"_:
			return visitCall(ast);
		case "Instantiation"_:
			return visitInstantiation(ast);
		case "Conditional"_:
			return visitConditional(ast);
		case "Array"_:
			return visitArray(ast);
		case "Tuple"_:
			return visitTuple(ast);
		case "Group"_:
			return visitGroup(ast);
		case "Expansion"_:
			return visitExpansion(ast);
		case "Local"_:
			return visitLocal(ast);
		case "Tail"_:
			return visitTail(ast);
		}

		throw std::exception();
	}

	expression_t ast_transformer::visitLiteral(const std::shared_ptr<peg::Ast>& ast) const
	{
		using namespace peg::udl;

		switch (ast->tag)
		{
		case "Literal"_:
			return visitLiteral(ast->nodes[0]);
		case "Int32Literal"_:
			return visitInt32Literal(ast);
		case "Int64Literal"_:
			return visitInt64Literal(ast);
		case "Float32Literal"_:
			return visitFloat32Literal(ast);
		case "BooleanLiteral"_:
			return visitBooleanLiteral(ast);
		case "StringLiteral"_:
			return visitStringLiteral(ast);
		case "Uint32Literal"_:
			return visitUint32Literal(ast);
		case "CharLiteral"_:
			return visitCharLiteral(ast);
		case "ByteLiteral"_:
			return visitByteLiteral(ast);
		case "NothingLiteral"_:
			return visitNothingLiteral(ast);
		}

		throw std::exception();
	}

	expression_t ast_transformer::visitInt32Literal(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new integral_literal_node(*m_alpha, default_type_name::int32(), visitInteger(ast->nodes[0]));
	}

	expression_t ast_transformer::visitInt64Literal(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new integral_literal_node(*m_alpha, default_type_name::int64(), visitInteger(ast->nodes[0]));
	}

	expression_t ast_transformer::visitFloat32Literal(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new float_literal_node(*m_alpha, default_type_name::f32(), ast->token_to_string());
	}

	expression_t ast_transformer::visitBooleanLiteral(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new boolean_literal_node(*m_alpha, ast->token_to_string());
	}

	expression_t ast_transformer::visitStringLiteral(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new string_literal_node(*m_alpha, visitString(ast->nodes[0]));
	}

	expression_t ast_transformer::visitUint32Literal(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new integral_literal_node(*m_alpha, default_type_name::uint32(), visitInteger(ast->nodes[0]));
	}

	expression_t ast_transformer::visitCharLiteral(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new character_literal_node(*m_alpha, visitString(ast));
	}

	expression_t ast_transformer::visitByteLiteral(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new integral_literal_node(*m_alpha, default_type_name::byte(), visitInteger(ast->nodes[0]));
	}

	expression_t ast_transformer::visitNothingLiteral(const std::shared_ptr<peg::Ast>& ast) const
	{
		UNUSED(ast);

		return new nothing_node(*m_alpha);
	}

	expression_t ast_transformer::visitBinary(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto binary = new binary_node();
		binary->ast = ast;
		binary->op = visitId(ast->nodes[1]);
		binary->consumer = *m_alpha;

		ast_transformer leftVisitor(m_namespaces, m_iteratorType, m_parent, binary, {});

		auto left = leftVisitor.visitExpression(ast->nodes[0]);

		ast_transformer rightVisitor(m_namespaces, m_iteratorType, m_parent, left, {});

		auto right = rightVisitor.visitExpression(ast->nodes[2]);

		return right;
	}

	expression_t ast_transformer::visitMember(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto access = visitIdList(ast->nodes[0]);

		auto path = std::accumulate(begin(access) + 1, end(access), (*begin(access)), [](auto name, std::string node) {
			std::stringstream ss;
			ss << name << "." << node;

			return ss.str();
		});

		auto qualifiedPath = qualifiedNames(m_namespaces, path);

		if (ast->nodes.size() > 1)
		{
			auto with = visitWith(ast->nodes[1]);

			return new member_access_node(with, ast, qualifiedPath);
		}

		return new member_access_node(*m_alpha, ast, qualifiedPath);
	}

	expression_t ast_transformer::visitCall(const std::shared_ptr<peg::Ast>& ast) const
	{
		ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, terminator_node::instance(), {});

		std::vector<expression_t> values;

		std::transform(begin(ast->nodes) + 1, end(ast->nodes), std::back_inserter(values), [&](auto node) {
			return visitor.visitExpression(node);
		});

		auto names = qualifiedNames(m_namespaces, visitId(ast->nodes[0]));

		return new function_call_node(ast, values, names, *m_alpha);
	}

	expression_t ast_transformer::visitInstantiation(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<std::string> fields;

		std::transform(begin(ast->nodes) + 1, end(ast->nodes), std::back_insert_iterator(fields), [this](auto assignment) {
			return visitId(assignment->nodes[0]);
		});

		auto inst = new instantiation_node { .consumer = *m_alpha,
											 .prototype = default_prototype { .type = visitTypeName(ast->nodes[0]) },
											 .ast = ast,
											 .fields = fields };

		return std::
			accumulate(begin(ast->nodes) + 1, end(ast->nodes), expression_t { inst }, [this](auto consumer, auto assignment) {
				ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, consumer, {});

				return visitor.visitExpression(assignment->nodes[1]);
			});
	}

	expression_t ast_transformer::visitConditional(const std::shared_ptr<peg::Ast>& ast) const
	{
		ast_transformer blockVisitor(m_namespaces, m_iteratorType, m_parent, m_beta, {});

		auto block = blockVisitor.visitBlock(ast->nodes[1]);

		auto conditional = new conditional_node(ast, block, *m_alpha);

		ast_transformer expressionVisitor(m_namespaces, m_iteratorType, m_parent, conditional, {});

		auto condition = expressionVisitor.visitExpression(ast->nodes[0]);

		return new block_instruction_node(condition, block->contains_iterator);
	}

	expression_t ast_transformer::visitArray(const std::shared_ptr<peg::Ast>& ast) const
	{
		if (ast->nodes.empty())
		{
			return new empty_array_node(terminator_node::instance());
		}

		if (ast->nodes.size() == 1)
		{
			return visitExpression(ast->nodes[0]);
		}

		std::vector<indexed<std::shared_ptr<peg::Ast>>> indexed;

		std::transform(
			begin(ast->nodes),
			end(ast->nodes),
			index_iterator(0u),
			std::back_insert_iterator(indexed),
			[=](auto x, auto y) -> fluenc::indexed<std::shared_ptr<peg::Ast>> {
				return { y, x };
			}
		);

		auto firstValue = std::accumulate(
			begin(indexed),
			end(indexed),
			expression_t { terminator_node::instance() },
			[&](auto next, fluenc::indexed<std::shared_ptr<peg::Ast>> expression) {
				auto indexSink = new index_sink_node(expression.index, next);
				auto referenceSink = new reference_sink_node(indexSink);

				ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, referenceSink, {});

				return visitor.visitExpression(expression.value);
			}
		);

		return new array_sink_node(ast->nodes.size(), ast, *m_alpha, firstValue);
	}

	expression_t ast_transformer::visitTuple(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto sink = new tuple_sink_node(ast->nodes.size(), *m_alpha);

		return std::accumulate(begin(ast->nodes), end(ast->nodes), expression_t { sink }, [&](auto consumer, auto value) {
			ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, consumer, {});

			return visitor.visitExpression(value);
		});
	}

	expression_t ast_transformer::visitGroup(const std::shared_ptr<peg::Ast>& ast) const
	{
		return visitExpression(ast->nodes[0]);
	}

	expression_t ast_transformer::visitExpansion(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto exp = new expansion_node(*m_alpha, ast);

		ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, exp, {});

		return visitor.visitExpression(ast->nodes[0]);
	}

	expression_t ast_transformer::visitLocal(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto loc = new local_node(*m_alpha, visitId(ast->nodes[0]));

		ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, loc, {});

		return visitor.visitExpression(ast->nodes[1]);
	}

	expression_t ast_transformer::visitContinuation(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto continuation = terminator_node::instance();
		auto sink = new reference_sink_node(continuation);

		return std::accumulate(
			begin(ast->nodes) + 1,
			end(ast->nodes),
			expression_t { sink },
			[this](const expression_t& consumer, auto parameter) {
				ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, consumer, {});

				return visitor.visitExpression(parameter);
			}
		);
	}

	expression_t ast_transformer::visitWith(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<std::string> fields;

		std::transform(begin(ast->nodes), end(ast->nodes), std::back_insert_iterator(fields), [this](auto assignment) {
			return visitId(assignment->nodes[0]);
		});

		auto inst = new instantiation_node {
			.consumer = *m_alpha,
			.prototype = with_prototype {},
			.ast = ast,
			.fields = fields,
		};

		return std::
			accumulate(begin(ast->nodes), end(ast->nodes), expression_t { inst }, [this](auto consumer, auto assignment) {
				ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, consumer, {});

				return visitor.visitExpression(assignment->nodes[1]);
			});
	}

	expression_t ast_transformer::visitUnary(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto unary = new unary_node {
			.ast = ast,
			.consumer = *m_alpha,
			.op = visitId(ast->nodes[0]),
		};

		ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, unary, {});

		return visitor.visitExpression(ast->nodes[1]);
	}

	expression_t ast_transformer::visitTail(const std::shared_ptr<peg::Ast>& ast) const
	{
		ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, terminator_node::instance(), {});

		std::vector<expression_t> values;

		std::transform(begin(ast->nodes) + 1, end(ast->nodes), back_inserter(values), [&](auto node) {
			return visitor.visitExpression(node);
		});

		auto names = qualifiedNames(m_namespaces, visitId(ast->nodes[0]));

		return new tail_function_call_node(ast, values, names, *m_alpha);
	}

	function_node* ast_transformer::visitFunction(const std::shared_ptr<peg::Ast>& ast) const
	{
		using namespace peg::udl;

		switch (ast->tag)
		{
		case "Function"_:
			return visitFunction(ast->nodes[0]);
		case "RegularFunction"_:
			return visitRegularFunction(ast);
		case "ImportedFunction"_:
			return visitImportedFunction(ast);
		case "ExportedFunction"_:
			return visitExportedFunction(ast);
		}

		throw std::exception();
	}

	function_node* ast_transformer::visitRegularFunction(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto name = visitId(ast->nodes[0]);
		auto qualifiedName = qualifiedNames(m_namespaces, name);

		std::vector<argument_t> arguments;

		std::transform(begin(ast->nodes) + 1, end(ast->nodes) - 1, std::back_inserter(arguments), [this](auto argument) {
			return visitArgument(argument);
		});

		auto it = new iterator();

		ast_transformer visitor(m_namespaces, it, m_parent, terminator_node::instance(), {});

		auto block = visitor.visitBlock(*ast->nodes.rbegin());

		return new function_node {
			.signature = {
				.arguments = arguments,
				.name = name,
			},
			.ast = ast,
			.block = block,
		};
	}

	function_node* ast_transformer::visitExportedFunction(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<argument_t> arguments;

		std::transform(begin(ast->nodes) + 2, end(ast->nodes) - 1, std::back_inserter(arguments), [this](auto argument) {
			return visitArgument(argument);
		});

		ast_transformer visitor(m_namespaces, nullptr, m_parent, terminator_node::instance(), {});

		auto block = visitor.visitBlock(*ast->nodes.rbegin());

		return new function_node {
			.signature = {
				.return_type = visitTypeName(ast->nodes[0]),
				.arguments = arguments,
				.name = visitId(ast->nodes[1]),
			},
			.type = function_type::exported,
			.block = block,
		};
	}

	function_node* ast_transformer::visitImportedFunction(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<argument_t> arguments;

		std::transform(begin(ast->nodes) + 2, end(ast->nodes), std::back_inserter(arguments), [this](auto argument) {
			return visitArgument(argument);
		});

		return new function_node {
			.signature = {
				.return_type = visitTypeName(ast->nodes[0]),
				.arguments = arguments,
				.name = visitId(ast->nodes[1]),
			},
			.type = function_type::imported,
			.ast = ast,
		};
	}

	argument_t ast_transformer::visitArgument(const std::shared_ptr<peg::Ast>& ast) const
	{
		using namespace peg::udl;

		switch (ast->tag)
		{
		case "Argument"_:
			return visitArgument(ast->nodes[0]);
		case "StandardArgument"_:
			return visitStandardArgument(ast);
		case "TupleArgument"_:
			return visitTupleArgument(ast);
		}

		throw std::exception();
	}

	argument_t ast_transformer::visitStandardArgument(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto type = visitTypeName(ast->nodes[0]);
		auto name = visitId(ast->nodes[1]);

		return new default_argument(name, type);
	}

	argument_t ast_transformer::visitTupleArgument(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<argument_t> arguments;

		std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(arguments), [this](auto node) {
			return visitArgument(node);
		});

		return new tuple_argument(arguments);
	}

	struct_node* ast_transformer::visitStructure(const std::shared_ptr<peg::Ast>& ast) const
	{
		return new struct_node { .ast = ast,
								 .tag = visitId(ast->nodes[0]),
								 .fields = visitFieldList(ast->nodes[2]),
								 .parent_types = visitTypeList(ast->nodes[1]) };
	}

	global_node* ast_transformer::visitGlobal(const std::shared_ptr<peg::Ast>& ast) const
	{
		ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, terminator_node::instance(), {});

		auto literal = visitor.visitExpression(ast->nodes[1]);

		return new global_node(ast, literal, visitId(ast->nodes[0]));
	}

	namespace_t* ast_transformer::visitNamespace(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto name = ast->nodes[0]->token_to_string();
		auto namespaces = m_namespaces;

		namespaces.push_back(name);

		ast_transformer visitor(namespaces, m_iteratorType, m_parent, m_alpha, m_beta);

		std::vector<std::any> children;

		std::transform(begin(ast->nodes) + 1, end(ast->nodes), std::back_inserter(children), [&](auto instruction) {
			return visitor.visitInstruction(instruction);
		});

		return new namespace_t(children, name);
	}

	block_instruction_node* ast_transformer::visitReturn(const std::shared_ptr<peg::Ast>& ast) const
	{
		if (ast->nodes.size() > 1)
		{
			auto continuation = visitContinuation(ast->nodes[1]);

			auto ret = new return_node(m_iteratorType, *m_alpha, continuation);

			ast_transformer visitor(m_namespaces, nullptr, m_parent, ret, {});

			auto value = visitor.visitExpression(ast->nodes[0]);

			return new block_instruction_node(value, true);
		}

		auto ret = new return_node(m_iteratorType, *m_alpha, {});

		ast_transformer visitor(m_namespaces, nullptr, m_parent, ret, {});

		auto value = visitor.visitExpression(ast->nodes[0]);

		return new block_instruction_node(value, false);
	}

	block_instruction_node* ast_transformer::visitBlock(const std::shared_ptr<peg::Ast>& ast) const
	{
		ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, m_alpha, m_beta);

		auto first = rbegin(ast->nodes);

		auto ret = visitor.visitReturn(*first);

		return std::accumulate(first + 1, rend(ast->nodes), ret, [&](auto consumer, auto expression) {
			ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, consumer, m_alpha);

			auto value = visitor.visitExpression(expression);

			if (auto instruction = ast_cast<block_instruction_node>(value))
			{
				return new block_instruction_node(value, instruction->contains_iterator || consumer->contains_iterator);
			}

			return new block_instruction_node(value, consumer->contains_iterator);
		});
	}

	type_name_t ast_transformer::visitTypeName(const std::shared_ptr<peg::Ast>& ast) const
	{
		using namespace peg::udl;

		switch (ast->tag)
		{
		case "TypeName"_:
			return visitTypeName(ast->nodes[0]);
		case "RegularType"_:
			return visitRegularType(ast);
		case "FunctionType"_:
			return visitFunctionType(ast);
		}

		throw std::exception(); // TODO
	}

	type_name_t ast_transformer::visitRegularType(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto qualifiedName = qualifiedNames(m_namespaces, visitId(ast->nodes[0]));

		return new default_type_name { .ast = ast, .names = qualifiedName };
	}

	type_name_t ast_transformer::visitFunctionType(const std::shared_ptr<peg::Ast>& ast) const
	{
		std::vector<type_name_t> types;

		std::transform(begin(ast->nodes), end(ast->nodes), std::back_inserter(types), [this](auto typeName) {
			return visitRegularType(typeName);
		});

		return new function_type_name { .types = types };
	}

	struct_field_t ast_transformer::visitField(const std::shared_ptr<peg::Ast>& ast) const
	{
		using namespace peg::udl;

		switch (ast->tag)
		{
		case "Field"_:
			return visitField(ast->nodes[0]);
		case "StandardField"_:
			return visitStandardField(ast);
		case "DecoratedField"_:
			return visitDecoratedField(ast);
		}

		throw std::exception();
	}

	struct_field_t ast_transformer::visitStandardField(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto name = visitId(ast->nodes[0]);

		if (ast->nodes.size() > 1)
		{
			ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, terminator_node::instance(), {});

			auto defaultValue = visitor.visitExpression(ast->nodes[1]);

			return { name, defaultValue, {} };
		}

		return { name, {}, {} };
	}

	struct_field_t ast_transformer::visitDecoratedField(const std::shared_ptr<peg::Ast>& ast) const
	{
		auto type = visitTypeName(ast->nodes[0]);
		auto name = visitId(ast->nodes[1]);

		if (ast->nodes.size() > 2)
		{
			ast_transformer visitor(m_namespaces, m_iteratorType, m_parent, terminator_node::instance(), {});

			auto defaultValue = visitor.visitExpression(ast->nodes[2]);

			return { name, defaultValue, type };
		}

		return { name, {}, type };
	}
}
