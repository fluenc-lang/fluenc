#include <llvm/IR/Verifier.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>

#include <fmt/format.h>
#include <range/v3/view.hpp>

#include <unordered_map>
#include <unordered_set>

#include "ast.hpp"
#include "emitter.hpp"
#include "function_helper.hpp"
#include "interop_helper.hpp"
#include "ir_builder.hpp"
#include "type_compatibility_calculator.hpp"
#include "utility.hpp"
#include "value_helper.hpp"

#include "fragments/continuation_fragment.hpp"
#include "fragments/expression_adapter_fragment.hpp"
#include "fragments/function_call_fragment.hpp"
#include "fragments/index_sink_fragment.hpp"
#include "fragments/reference_sink_fragment.hpp"
#include "fragments/stack_segment_fragment.hpp"
#include "fragments/string_iterator_fragment.hpp"
#include "fragments/terminator_fragment.hpp"

#include "types/boolean_type.hpp"
#include "types/default_prototype.hpp"
#include "types/expanded_type.hpp"
#include "types/int32_type.hpp"
#include "types/int64_type.hpp"
#include "types/iterator_type.hpp"
#include "types/operator_set.hpp"
#include "types/prototype_field.hpp"
#include "types/string_type.hpp"
#include "types/structure_type.hpp"
#include "types/user_type.hpp"

#include "generators/array_iterator_generator.hpp"
#include "generators/iterator_generator.hpp"

#include "values/buffer_value.hpp"
#include "values/expandable_value.hpp"
#include "values/expanded_lazy_value.hpp"
#include "values/expanded_value.hpp"
#include "values/function_value.hpp"
#include "values/named_value.hpp"
#include "values/placeholder_value.hpp"
#include "values/post_lazy_value.hpp"
#include "values/pre_lazy_value.hpp"
#include "values/reference_value.hpp"
#include "values/scalar_value.hpp"
#include "values/string_value.hpp"
#include "values/tuple_value.hpp"
#include "values/user_type_value.hpp"
#include "values/without_value.hpp"

#include "exceptions/ambiguous_function_exception.hpp"
#include "exceptions/binary_type_mismatch_exception.hpp"
#include "exceptions/function_not_found_exception.hpp"
#include "exceptions/invalid_argument_type_exception.hpp"
#include "exceptions/invalid_operator_exception.hpp"
#include "exceptions/missing_default_value_exception.hpp"
#include "exceptions/missing_field_exception.hpp"
#include "exceptions/undeclared_identifier_exception.hpp"

#include "fragments/array_continuation_fragment.hpp"
#include "fragments/default_fragment.hpp"
#include "fragments/junction_fragment.hpp"

namespace fluenc::code_generation
{
	std::vector<emitter_result> emitter::visit(const fluenc::binary_node* node, emitter_context context) const
	{
		auto values = context.values;

		auto left = values.pop();
		auto right = values.pop();

		auto left_type = left->type();
		auto right_type = right->type();

		if (type_compatibility_calculator::calculate(context.entryPoint, left_type, right_type) > 1)
		{
			throw binary_type_mismatch_exception(node->ast, left_type->name(), right_type->name());
		}

		auto operators = left_type->operators();

		if (!operators)
		{
			auto operand_type_name = left_type->name();

			throw invalid_operator_exception(node->ast, node->op, operand_type_name);
		}

		auto binary = operators->for_binary(node);

		std::vector<emitter_result> results;

		for (auto& [result_entry_point, result_values] : binary->build(*this, context))
		{
			for (auto& result : fluenc::accept(node->consumer, *this, { result_entry_point, result_values }))
			{
				results.push_back(result);
			}
		}

		return results;
	}

	std::vector<emitter_result> emitter::visit(const fluenc::integral_literal_node* node, emitter_context context) const
	{
		auto llvm_context = context.entryPoint.context();

		auto type = type_for(node->type, context.entryPoint);
		auto storage_type = type->storage_type(*llvm_context);
		auto integer_type = static_cast<llvm::IntegerType*>(storage_type);

		auto value_provider = [&] {
			auto hex = node->value.find("0x");

			if (hex != std::string::npos)
			{
				return llvm::ConstantInt::get(integer_type, node->value.substr(hex + 2), 16);
			}

			return llvm::ConstantInt::get(integer_type, node->value, 10);
		};

		auto value = new values::scalar_value(type, value_provider());

		context.values.push(value);

		return fluenc::accept(node->consumer, *this, context);
	}

	std::vector<emitter_result> emitter::visit(const fluenc::float_literal_node* node, emitter_context context) const
	{
		auto llvm_context = context.entryPoint.context();

		auto type = type_for(node->type, context.entryPoint);
		auto storage_type = type->storage_type(*llvm_context);

		auto value = new values::scalar_value(type, llvm::ConstantFP::get(storage_type, node->value));

		context.values.push(value);

		return fluenc::accept(node->consumer, *this, context);
	}

	std::vector<emitter_result> emitter::visit(const fluenc::boolean_literal_node* node, emitter_context context) const
	{
		auto llvm_context = context.entryPoint.context();

		auto value_provider = [&] {
			auto type = types::boolean_type::instance();
			auto storage_type = type->storage_type(*llvm_context);

			if (node->value == "true")
			{
				return new values::scalar_value { type, llvm::ConstantInt::get(storage_type, 1) };
			}

			if (node->value == "false")
			{
				return new values::scalar_value { type, llvm::ConstantInt::get(storage_type, 0) };
			}

			throw std::exception(); // TODO
		};

		auto value = value_provider();

		context.values.push(value);

		return fluenc::accept(node->consumer, *this, context);
	}

	std::vector<emitter_result> emitter::visit(const fluenc::string_literal_node* node, emitter_context context) const
	{
		ir_builder builder(context.entryPoint);

		auto llvm_context = context.entryPoint.context();

		auto string_type = types::string_type::instance();

		auto string = new values::scalar_value(string_type, builder.create_global_string_ptr(node->value, "string"));

		auto length_type = types::int64_type::instance();
		auto length_storage_type = length_type->storage_type(*llvm_context);

		auto alloc = context.entryPoint.alloc(string_type);
		auto length_storage = context.entryPoint.alloc(length_type);

		auto length = new values::scalar_value(length_type, llvm::ConstantInt::get(length_storage_type, node->value.size()));

		builder.create_store(string, alloc);
		builder.create_store(length, length_storage);

		context.values.push(new values::string_value(alloc, length_storage));

		return fluenc::accept(node->consumer, *this, context);
	}

	std::vector<emitter_result> emitter::visit(const fluenc::character_literal_node* node, emitter_context context) const
	{
		auto llvm_context = context.entryPoint.context();

		auto type = types::int32_type::instance();
		auto storage_type = type->storage_type(*llvm_context);

		auto value = new values::
			scalar_value(type, llvm::ConstantInt::get(static_cast<llvm::IntegerType*>(storage_type), *node->value.begin()));

		context.values.push(value);

		return fluenc::accept(node->consumer, *this, context);
	}

	std::vector<emitter_result> emitter::visit(const fluenc::nothing_node* node, emitter_context context) const
	{
		context.values.push(values::without_value::instance());

		return fluenc::accept(node->consumer, *this, context);
	}

	std::vector<emitter_result> emitter::visit(const fluenc::member_access_node* node, emitter_context context) const
	{
		auto& locals = context.entryPoint.locals();
		auto& functions = context.entryPoint.functions();
		auto& globals = context.entryPoint.globals();

		for (auto& name : node->names)
		{
			auto locals_iterator = locals.find(name);

			if (locals_iterator != locals.end())
			{
				if (auto reference_value = value_cast<const values::reference_value*>(locals_iterator->second))
				{
					ir_builder builder(context.entryPoint);

					auto load = builder.create_load(reference_value);

					context.values.push(load);
				}
				else if (locals_iterator->second)
				{
					context.values.push(locals_iterator->second);
				}

				return fluenc::accept(node->consumer, *this, context);
			}

			auto functions_iterator = functions.equal_range(name);

			std::vector<fluenc::function_node*> matching_functions;

			std::transform(functions_iterator.first, functions_iterator.second, back_inserter(matching_functions), [](auto pair) {
				return pair.second;
			});

			if (matching_functions.size() > 0)
			{
				auto value = new values::function_value(matching_functions, context.entryPoint);

				context.values.push(value);

				return fluenc::accept(node->consumer, *this, context);
			}

			auto globals_iterator = globals.find(name);

			if (globals_iterator != globals.end())
			{
				std::vector<emitter_result> results;

				for (auto& [result_entry_point, result_values] :
					 fluenc::accept(globals_iterator->second, *this, context))
				{
					for (auto& result : fluenc::accept(node->consumer, *this, { result_entry_point, result_values }))
					{
						results.push_back(result);
					}
				}

				return results;
			}
		}

		throw undeclared_identifier_exception(node->ast, node->names[0]);
	}

	std::vector<emitter_result> emitter::visit(const fluenc::function_call_node* node, emitter_context context) const
	{
		if (node->type == fluenc::function_call_type::regular)
		{
			auto llvm_context = context.entryPoint.context();

			std::vector<emitter_result> results;

			auto preliminary_block = create_block(llvm_context);

			auto preliminary_entry_point = new entry_point(context.entryPoint.with_block(preliminary_block));

			auto call = new fragments::function_call_fragment(node->names, node->ast);
			auto segment = new fragments::stack_segment_fragment(node->values, call, fluenc::terminator_node::instance());

			auto preliminary_results = segment->build(*this, { preliminary_entry_point->detach(), context.values });

			for (auto& [_, preliminary_values] : preliminary_results)
			{
				if (preliminary_values.size() <= 0)
				{
					continue;
				}

				auto return_value = preliminary_values.peek();

				if (return_value->id() == value_id::post_lazy)
				{
					auto generator = new generators::iterator_generator(
						new types::iterator_type(),
						preliminary_results,
						preliminary_entry_point,
						segment,
						context.entryPoint
					);
					auto lazy = new values::pre_lazy_value(generator, context.entryPoint);

					auto forwarded_values = context.values;

					forwarded_values.push(lazy);

					std::vector<emitter_result> results;

					for (auto& result : fluenc::accept(node->consumer, *this, { context.entryPoint, forwarded_values }))
					{
						results.push_back(result);
					}

					return results;
				}
			}

			preliminary_entry_point->set_parent(context.entryPoint);

			link_blocks(context.entryPoint.block(), preliminary_block);

			for (auto& [result_entry_point, result_values] : preliminary_results)
			{
				auto consumer_entry_point = result_entry_point.with_index(context.entryPoint.index());

				for (auto& result : accept(node->consumer, *this, { consumer_entry_point, result_values }))
				{
					results.push_back(result);
				}
			}

			return results;
		}

		auto call = new fragments::function_call_fragment(node->names, node->ast);
		auto segment = new fragments::stack_segment_fragment(node->values, call, node->consumer);

		return segment->build(*this, context);
	}

	std::vector<emitter_result> emitter::visit(const fluenc::instantiation_node* node, emitter_context context) const
	{
		std::unordered_map<std::string, const base_value*> values_by_name;

		std::transform(
			begin(node->fields),
			end(node->fields),
			std::inserter(values_by_name, begin(values_by_name)),
			[&](auto field) {
				return std::make_pair(field, context.values.pop());
			}
		);

		auto [prototype, pep, prototype_fields] = std::visit(
			[&](auto&& prototype
			) -> std::tuple<const types::structure_type*, entry_point, std::vector<types::prototype_field>> {
				using type = std::decay_t<decltype(prototype)>;

				if constexpr (std::is_same_v<type, fluenc::default_prototype>)
				{
					auto structure_type = static_cast<const types::structure_type*>(
						type_for(prototype.type, context.entryPoint)
					);

					auto [ep, fields] = types::default_fields(context.entryPoint, *this, structure_type->subject());

					return { structure_type, ep, fields };
				}

				if constexpr (std::is_same_v<type, fluenc::with_prototype>)
				{
					auto address_of_value = require<values::user_type_value>(context.values, nullptr);

					auto fields = address_of_value->fields();

					std::vector<types::prototype_field> results;

					std::transform(
						begin(fields),
						end(fields),
						std::back_insert_iterator(results),
						[](auto field) -> types::prototype_field {
							return { field->name(), field->value(), field->type() };
						}
					);

					return { address_of_value->prototype(), context.entryPoint, results };
				}

				throw std::exception(); // TODO
			},
			node->prototype
		);

		auto prototype_entry_point = pep;

		std::vector<const values::named_value*> named_values;

		std::transform(
			begin(prototype_fields),
			end(prototype_fields),
			std::back_inserter(named_values),
			[&](auto field) -> const values::named_value* {
				if (auto value_by_name = values_by_name.find(field.name); value_by_name != values_by_name.end())
				{
					auto value = value_by_name->second;

					values_by_name.erase(value_by_name);

					return new values::named_value { field.name, value };
				}

				if (!field.default_value)
				{
					throw missing_default_value_exception(node->ast, field.name);
				}

				return new values::named_value { field.name, field.default_value };
			}
		);

		for (auto& [name, _] : values_by_name)
		{
			throw missing_field_exception(node->ast, prototype->name(), name);
		}

		ir_builder builder(prototype_entry_point);

		std::vector<const values::named_value*> final_values;

		std::transform(
			begin(named_values),
			end(named_values),
			std::back_inserter(final_values),
			[&](auto field) -> const values::named_value* {
				auto value = field->value();

				if (auto typed_value = value_cast<const values::scalar_value*>(value))
				{
					auto type = typed_value->type();

					auto alloc = prototype_entry_point.alloc(type);

					builder.create_store(typed_value, alloc);

					return new values::named_value { field->name(), alloc };
				}

				return field;
			}
		);

		auto user_type_value = new values::user_type_value(prototype, final_values);

		context.values.push(user_type_value);

		return fluenc::accept(node->consumer, *this, { prototype_entry_point, context.values });
	}

	std::vector<emitter_result> emitter::visit(const fluenc::conditional_node* node, emitter_context context) const
	{
		struct single_result
		{
			const entry_point entryPoint;
			const values::scalar_value* value;
		};

		auto llvm_context = context.entryPoint.context();
		auto module = context.entryPoint.module();

		auto block = context.entryPoint.block();

		auto dataLayout = module->getDataLayout();

		block->setName("condition");

		auto if_true = create_block(llvm_context);
		auto if_false = create_block(llvm_context);

		ir_builder builder(context.entryPoint);

		auto conditional = value_helper::get_scalar(node->ast, context.entryPoint, context.values);

		builder.create_cond_br(conditional, if_true, if_false);

		auto ep_if_false = context.entryPoint.with_name("ifFalse").with_block(if_false);

		auto ep_if_true = context.entryPoint.with_name("ifTrue").with_block(if_true);

		auto results_if_true = fluenc::accept(node->if_true, *this, { ep_if_true, context.values });
		auto results_if_false = fluenc::accept(node->if_false, *this, { ep_if_false, context.values });

		std::vector<emitter_result> immediate_results;

		immediate_results.insert(end(immediate_results), begin(results_if_true), end(results_if_true));
		immediate_results.insert(end(immediate_results), begin(results_if_false), end(results_if_false));

		std::multimap<const base_type*, single_result> grouped_results;

		for (auto& result : immediate_results)
		{
			auto [result_entry_point, result_values] = result;

			if (result_values.size() != 1)
			{
				continue;
			}

			auto value = request<values::scalar_value>(result_values);

			if (value)
			{
				grouped_results.insert({ value->type(), { result_entry_point, value } });
			}
			else
			{
				return immediate_results;
			}
		}

		std::vector<emitter_result> merged_results;

		for (auto it = begin(grouped_results); it != end(grouped_results);
			 it = upper_bound(it, end(grouped_results), *it, &compare_key<const base_type*, single_result>))
		{
			auto [type, _] = *it;

			auto range = grouped_results.equal_range(type);

			auto alloc = context.entryPoint.alloc(type);

			auto merge_block = llvm::BasicBlock::Create(*llvm_context, "merge");

			for (auto i = range.first; i != range.second; i++)
			{
				auto [result_entry_point, value] = i->second;

				auto result_block = result_entry_point.block();

				ir_builder result_builder(result_entry_point);

				result_builder.create_store(value, alloc);

				link_blocks(result_block, merge_block);
			}

			auto merge_entry_point = context.entryPoint.with_block(merge_block);

			ir_builder merge_builder(merge_entry_point);

			auto merge_load = merge_builder.create_load(alloc, "mergeLoad");

			auto merge_values = context.values;

			merge_values.push(merge_load);

			merged_results.push_back({ merge_entry_point, merge_values });
		}

		return merged_results;
	}

	std::vector<emitter_result> emitter::visit(const fluenc::block_instruction_node* node, emitter_context context) const
	{
		return fluenc::accept(node->subject, *this, { context.entryPoint, value_stack_t() });
	}

	std::vector<emitter_result> emitter::visit(const array_node* node, emitter_context context) const
	{
		auto array_contents = std::accumulate(
			rbegin(node->elements),
			rend(node->elements),
			std::vector<emitter_result> { { context.entryPoint, value_stack_t() } },
			[&](auto previous, fluenc::indexed<expression_t> expression) -> std::vector<emitter_result> {
				std::vector<emitter_result> results;

				for (auto [result_entry_point, result_values] : previous)
				{
					fragments::index_sink_fragment
						index_sink(expression.index, fragments::terminator_fragment::instance());
					fragments::reference_sink_fragment reference_sink(&index_sink);

					for (auto& [uep, uval] :
						 fluenc::accept(expression.value, *this, { result_entry_point, result_values }))
					{
						for (auto& result : reference_sink.build(*this, { uep, uval }))
						{
							results.push_back(result);
						}
					}
				}

				return results;
			}
		);

		auto generator = new generators::array_iterator_generator(array_contents, node->ast, size(node->elements));
		auto lazy = new values::pre_lazy_value(generator, context.entryPoint);

		context.values.push(lazy);

		std::vector<emitter_result> results;

		for (auto& [arrayEntryPoint, _] : array_contents)
		{
			for (auto& result : fluenc::accept(node->consumer, *this, { arrayEntryPoint, context.values }))
			{
				results.push_back(result);
			}
		}

		return results;
	}

	std::vector<emitter_result> emitter::visit(const fluenc::expansion_node* node, emitter_context context) const
	{
		auto block = context.entryPoint.block();

		auto value = context.values.pop();

		if (auto expandable = value_cast<const values::expandable_value*>(value))
		{
			auto continuation = expandable->chain();
			auto provider = expandable->provider();

			auto continuation_entry_point = (*provider).with_block(block).with_iterator_type(expandable->expandedType());

			auto result = continuation->build(*this, { continuation_entry_point, value_stack_t() });

			for (auto& [target_entry_point, continuation_values] : result)
			{
				auto value = require<values::expanded_value>(continuation_values, nullptr);

				auto tuple = new values::tuple_value({ value, values::placeholder_value::instance() });

				context.values.push(tuple);

				auto consumer_entry_point = context.entryPoint.with_block(target_entry_point.block());

				return fluenc::accept(node->consumer, *this, { consumer_entry_point, context.values });
			}

			throw std::exception();
		}
		else if (auto lazy = value_cast<const values::pre_lazy_value*>(value))
		{
			context.values.push(new values::expanded_lazy_value(lazy));

			return fluenc::accept(node->consumer, *this, context);
		}
		else if (auto string = value_cast<const values::string_value*>(value))
		{
			context.values.push(new values::expanded_lazy_value(string->iterator(context.entryPoint)));

			return fluenc::accept(node->consumer, *this, context);
		}
		else if (auto buffer = value_cast<const values::buffer_value*>(value))
		{
			context.values.push(new values::expanded_lazy_value(buffer->iterator(context.entryPoint)));

			return fluenc::accept(node->consumer, *this, context);
		}
		else
		{
			context.values.push(value);

			return fluenc::accept(node->consumer, *this, context);
		}
	}

	std::vector<emitter_result> emitter::visit(const local_node* node, emitter_context context) const
	{
		auto locals = context.entryPoint.locals();

		auto value = context.values.pop();

		if (auto userValue = value_cast<const values::user_type_value*>(value))
		{
			auto fields = userValue->fields();

			std::transform(
				begin(fields),
				end(fields),
				std::inserter(locals, begin(locals)),
				[=](auto field) -> std::pair<std::string, const base_value*> {
					return { fmt::format("{}.{}", node->name, field->name()), field->value() };
				}
			);
		}

		if (auto lazy_value = value_cast<const values::pre_lazy_value*>(value))
		{
			auto type = lazy_value->type();

			// Optimization: If iteration can be represented as an array, create
			// a new, temporary array and copy the iteration result into it.
			// This will ensure that multiple iterations are cheap.
			//
			if (type->id() == type_id::array)
			{
				auto allocator = new fragments::default_fragment(type, fragments::terminator_fragment::instance());

				for (auto& result : allocator->build(*this, { context.entryPoint, value_stack_t() }))
				{
					auto array = require<values::pre_lazy_value>(result.values, nullptr);

					auto assignment_entry_point = array->assign_from(result.entryPoint, lazy_value, *this);

					locals[node->name] = array;

					auto ep = assignment_entry_point.with_locals(locals);

					return fluenc::accept(node->consumer, *this, { ep, context.values });
				}
			}
		}

		locals[node->name] = value;

		auto ep = context.entryPoint.with_locals(locals);

		return fluenc::accept(node->consumer, *this, { ep, context.values });
	}

	std::vector<emitter_result> emitter::visit(const fluenc::unary_node* node, emitter_context context) const
	{
		auto operand = context.values.peek();

		auto operand_type = operand->type();
		auto operators = operand_type->operators();

		if (!operators)
		{
			auto operand_type_name = operand_type->name();

			throw invalid_operator_exception(node->ast, node->op, operand_type_name);
		}

		auto unary = operators->for_unary(node);

		std::vector<emitter_result> results;

		for (auto& [result_entry_point, result_values] : unary->build(*this, context))
		{
			for (auto& result : fluenc::accept(node->consumer, *this, { result_entry_point, result_values }))
			{
				results.push_back(result);
			}
		}

		return results;
	}

	std::vector<emitter_result> emitter::visit(const fluenc::function_node* node, emitter_context context) const
	{
		if (node->type == fluenc::function_type::exported)
		{
			auto module = context.entryPoint.module();
			auto llvm_context = context.entryPoint.context();

			auto return_type = type_for(node->signature.return_type, context.entryPoint);
			auto storage_type = return_type->storage_type(*llvm_context);

			auto arguments = node->signature.arguments;

			std::vector<llvm::Type*> argument_types;

			std::transform(begin(arguments), end(arguments), back_inserter(argument_types), [&](auto argument) {
				auto type = arg_type(argument, context.entryPoint);

				return type->storage_type(*llvm_context);
			});

			auto function_type = llvm::FunctionType::get(storage_type, argument_types, false);
			auto function = llvm::Function::
				Create(function_type, llvm::Function::ExternalLinkage, node->signature.name, module);

			std::map<std::string, const base_value*> locals;

			std::transform(
				function->arg_begin(),
				function->arg_end(),
				begin(arguments),
				inserter(locals, begin(locals)),
				[&](auto& value, auto argument) -> std::pair<std::string, const base_value*> {
					auto standard_argument = std::get_if<const fluenc::default_argument*>(&argument);

					if (!standard_argument)
					{
						throw invalid_argument_type_exception(nullptr);
					}

					auto type = type_for((*standard_argument)->type, context.entryPoint);

					return { (*standard_argument)->name, new values::scalar_value(type, &value) };
				}
			);

			auto alloc = llvm::BasicBlock::Create(*llvm_context, "alloc");
			auto block = create_block(llvm_context);

			link_blocks(alloc, block);

			auto ep = context.entryPoint.with_function(function).with_block(block).with_alloc(alloc).with_locals(locals);

			auto junction = new fragments::junction_fragment(node->block);

			std::vector<emitter_result> result;

			for (auto& [rep, rva] : junction->build(*this, { ep, context.values }))
			{
				auto llvm_context = rep.context();
				auto previous = rep.block();

				auto block = llvm::BasicBlock::Create(*llvm_context, "entry");

				link_blocks(previous, block);

				auto ep = rep.with_block(block);

				auto return_value = value_helper::get_scalar(node->ast, ep, rva);

				ir_builder builder(ep);
				builder.create_ret(*return_value);

				result.push_back({ ep, rva });
			}

			ep.incorporate();

			llvm::FunctionAnalysisManager analysis_manager;

			llvm::PassBuilder pass_builder;
			pass_builder.registerFunctionAnalyses(analysis_manager);

			//	llvm::SimplifyCFGPass pass;
			//	pass.run(*function, analysisManager);

			verifyFunction(*function, &llvm::errs());

			return result;
		}

		if (node->type == fluenc::function_type::imported)
		{
			auto module = context.entryPoint.module();
			auto llvm_context = context.entryPoint.context();

			auto return_type = type_for(node->signature.return_type, context.entryPoint);

			std::vector<llvm::Type*> argument_types;
			std::vector<llvm::Value*> argument_values;

			ir_builder builder(context.entryPoint);

			for (const auto& argument : node->signature.arguments)
			{
				if (auto standard_argument = std::get_if<const fluenc::default_argument*>(&argument))
				{
					auto name = (*standard_argument)->name;
					auto type = type_for((*standard_argument)->type, context.entryPoint);

					auto storage_type = type->storage_type(*llvm_context);

					argument_types.push_back(storage_type);

					auto value = context.values.pop();

					if (auto address_of_argument = value_cast<const values::reference_value*>(value))
					{
						auto load = builder.create_load(address_of_argument, name);

						argument_values.push_back(*load);
					}
					else if (auto string_value = value_cast<const values::string_value*>(value))
					{
						auto load = builder.create_load(string_value->reference());

						argument_values.push_back(*load);
					}
					else if (auto buffer_value = value_cast<const values::buffer_value*>(value))
					{
						auto address = buffer_value->reference(context.entryPoint);

						argument_values.push_back(*address);
					}
					else if (auto user_type_value = value_cast<const values::user_type_value*>(value))
					{
						auto cast = interop_helper::create_write_proxy(user_type_value, context.entryPoint);

						argument_values.push_back(cast);
					}
				}
				else
				{
					throw invalid_argument_type_exception(node->ast);
				}
			}

			auto function_type = llvm::FunctionType::get(return_type->storage_type(*llvm_context), argument_types, false);

			auto function = module->getOrInsertFunction(node->signature.name, function_type);

			auto call = builder.create_call(function, argument_values);

			if (return_type->id() != type_id::void_interop)
			{
				auto [return_entry_point, return_value] = interop_helper::
					create_read_proxy(call, return_type, context.entryPoint, node->ast);

				context.values.push(return_value);
			}

			return { { context.entryPoint, context.values } };
		}

		using argument_result_t = std::pair<std::string, const base_value*>;

		auto handle_argument = [&](const fluenc::argument_t& argument, const base_value* value, auto& recurse
							   ) -> std::vector<argument_result_t> {
			return std::visit(
				[&](auto* arg) {
					using type = decltype(arg);

					if constexpr (std::is_same_v<type, const fluenc::tuple_argument*>)
					{
						auto tuple_value = value_cast<const values::tuple_value*>(value);

						auto tuple_values = tuple_value->values();

						std::vector<argument_result_t> results;

						for (auto argument : arg->arguments)
						{
							for (auto& result : recurse(argument, tuple_values.pop(), recurse))
							{
								results.push_back(result);
							}
						}

						return results;
					}

					if constexpr (std::is_same_v<type, const fluenc::default_argument*>)
					{
						std::vector<argument_result_t> result { { arg->name, value } };

						if (auto user_value = value_cast<const values::user_type_value*>(value))
						{
							auto fields = user_value->fields();

							std::transform(begin(fields), end(fields), std::back_inserter(result), [=](auto field) -> argument_result_t {
								return { fmt::format("{}.{}", arg->name, field->name()), field->value() };
							});
						}

						return result;
					}

					throw std::exception();
				},
				argument
			);
		};

		std::vector<const base_type*> types = { new types::iterator_type() };

		std::transform(context.values.begin(), context.values.end(), back_inserter(types), [](auto value) {
			return value->type();
		});

		auto pep = context.entryPoint.with_values(context.values).with_iterator_type(types::expanded_type::get(types));

		std::map<std::string, const base_value*> locals;

		for (const auto& argument : node->signature.arguments)
		{
			for (auto& [name, value] : handle_argument(argument, context.values.pop(), handle_argument))
			{
				locals[name] = value;
			}
		}

		auto ep = pep.with_name(node->signature.name).mark_entry().with_locals(locals);

		return fluenc::accept(node->block, *this, { ep, context.values });
	}

	std::vector<emitter_result> emitter::visit(const fluenc::global_node* node, emitter_context context) const
	{
		return fluenc::accept(node->value, *this, context);
	}

	std::vector<emitter_result> emitter::visit(const fluenc::return_node* node, emitter_context context) const
	{
		auto fetch_value = [&]() -> const base_value* {
			auto value = context.values.pop();

			if (auto typed_value = value_cast<const values::scalar_value*>(value))
			{
				ir_builder builder(context.entryPoint);

				auto type = value->type();

				auto alloc = context.entryPoint.alloc(type);

				builder.create_store(typed_value, alloc);

				return alloc;
			}

			return value;
		};

		auto value = fetch_value();

		if (node->chained)
		{
			auto values = context.entryPoint.values();

			std::vector<const base_type*> types = { types::iterator_type::get(*node->iterator) };

			std::transform(values.begin(), values.end(), back_inserter(types), [](auto value) {
				return value->type();
			});

			auto type = types::expanded_type::get(types);

			auto continuation = new fragments::continuation_fragment(
				new fragments::expression_adapter_fragment(*node->chained),
				new fragments::function_call_fragment(function_, {})
			);

			auto expandable = new values::expandable_value(false, type, context.entryPoint, continuation, values);
			auto tuple = new values::tuple_value({ expandable, value });

			context.values.push(tuple);

			return fluenc::accept(node->consumer, *this, context);
		}

		context.values.push(value);

		return fluenc::accept(node->consumer, *this, context);
	}

	std::vector<emitter_result> emitter::visit(const terminator_node* node, emitter_context context) const
	{
		UNUSED(node);

		return { { context.entryPoint, context.values } };
	}

	std::vector<emitter_result> emitter::visit(const tuple_sink_node* node, emitter_context context) const
	{
		std::vector<const base_value*> values;

		for (auto i = 0u; i < node->size; i++)
		{
			values.insert(begin(values), context.values.pop());
		}

		auto tuple = new values::tuple_value(values);

		context.values.push(tuple);

		return fluenc::accept(node->consumer, *this, context);
	}

	std::vector<emitter_result> emitter::visit(const struct_node* node, emitter_context context) const
	{
		UNUSED(node);
		UNUSED(context);

		return {};
	}
}
