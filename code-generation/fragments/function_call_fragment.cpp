#include "function_call_fragment.hpp"
#include "base_value.hpp"
#include "emitter.hpp"
#include "function_helper.hpp"
#include "post_evaluation_fragment.hpp"
#include "type_compatibility_calculator.hpp"
#include "utility.hpp"
#include "value_helper.hpp"

#include "ast/function_node.hpp"

#include "values/function_value.hpp"

#include "exceptions/ambiguous_function_exception.hpp"
#include "exceptions/function_not_found_exception.hpp"
#include "exceptions/invalid_function_pointer_type_exception.hpp"
#include "exceptions/missing_tail_call_exception.hpp"

#include <range/v3/view/zip.hpp>

#include <iostream>
#include <string>

namespace fluenc::code_generation::fragments
{
	int8_t signature_compatibility(
		const fluenc::function_node* callable,
		const entry_point& entryPoint,
		const std::vector<const base_type*>& values
	)
	{
		auto arguments = callable->signature.arguments;

		if (arguments.size() != values.size())
		{
			return -1;
		}

		std::vector<std::pair<const base_type*, const base_type*>> types;

		std::transform(
			begin(arguments),
			end(arguments),
			begin(values),
			std::back_inserter(types),
			[=](const fluenc::argument_t& argument, auto valueType) {
				auto argumentType = arg_type(argument, entryPoint);

				return std::make_pair(argumentType, valueType);
			}
		);

		return std::accumulate(begin(types), end(types), 0, [=](auto score, auto pair) -> int8_t {
			if (score < 0)
			{
				return score;
			}

			auto [argument_type, value_type] = pair;

			auto compatibility = type_compatibility_calculator::calculate(entryPoint, value_type, argument_type);

			if (compatibility < 0)
			{
				return compatibility;
			}

			return score + compatibility;
		});
	}

	std::vector<emitter_result> make_function_call(
		const std::shared_ptr<peg::Ast>& ast,
		const std::vector<std::string>& names,
		emitter_context context
	)
	{
		auto functions_for_name = [&](const std::string& name) {
			auto& functions = context.entryPoint.functions();
			auto& locals = context.entryPoint.locals();

			auto local = locals.find(name);

			if (local != locals.end())
			{
				auto value = value_cast<const values::function_value*>(local->second);

				if (!value)
				{
					throw invalid_function_pointer_type_exception(ast, name);
				}

				return value->functions();
			}

			auto matching_functions = functions.equal_range(name);

			std::vector<fluenc::function_node*> result;

			std::transform(matching_functions.first, matching_functions.second, back_inserter(result), [](auto pair) {
				return pair.second;
			});

			return result;
		};

		auto find_function = [&](const std::vector<const base_type*>& types
							 ) -> std::pair<fluenc::function_node*, exceptions::tried_t> {
			exceptions::tried_t tried;

			for (auto& name : names)
			{
				std::map<int8_t, fluenc::function_node*> candidates;

				for (auto& function : functions_for_name(name))
				{
					auto score = signature_compatibility(function, context.entryPoint, types);

					if (score < 0)
					{
						tried.push_back({ score, function });

						continue;
					}

					auto candidate = candidates.find(score);

					if (candidate != candidates.end())
					{
						std::vector<fluenc::function_node*> functions = { candidate->second, function };

						throw exceptions::ambiguous_function_exception(ast, functions, context.entryPoint);
					}

					candidates[score] = function;
				}

				if (candidates.size() > 0)
				{
					auto [score, function] = *candidates.begin();

					return { function, tried };
				}
			}

			return { nullptr, tried };
		};

		auto [score, _1, _2] = function_helper::
			try_create_tail_call(context.entryPoint, context.values, begin(names), end(names));

		if (score == 0)
		{
			throw missing_tail_call_exception(ast);
		}

		emitter nested(names);

		auto llvm_context = context.entryPoint.context();

		std::vector<emitter_result> result;

		auto evaluation = new post_evaluation_fragment();

		for (auto& [result_entry_point, result_values] : evaluation->build(nested, context))
		{
			auto block = result_entry_point.block();

			std::vector<const base_type*> types;

			std::transform(result_values.rbegin(), result_values.rend(), std::back_inserter(types), [](auto value) {
				return value->type();
			});

			auto [function, tried] = find_function(types);

			if (!function)
			{
				for (auto& [score, callable] : tried)
				{
					std::cout << static_cast<int32_t>(score) << " " << callable->signature.name << "(";

					auto arguments = callable->signature.arguments;

					for (auto i = begin(arguments); i != end(arguments); i++)
					{
						if (i != begin(arguments))
						{
							std::cout << ", ";
						}

						auto argument_type = arg_type(*i, context.entryPoint);

						std::cout << argument_type->name();
					}

					std::cout << ")" << std::endl;
				}

				throw exceptions::function_not_found_exception(ast, names[0], types, tried);
			}

			auto function_block = create_block(llvm_context);

			link_blocks(block, function_block);

			auto function_entry_point = result_entry_point.with_block(function_block);

			if (function->type == fluenc::function_type::imported)
			{
				return nested.visit(function, { function_entry_point, result_values });
			}

			auto function_results = nested.visit(function, { function_entry_point, result_values });

			for (const auto& [last_entry_point, return_value] : function_results)
			{
				auto consumer_block = create_block(llvm_context);

				link_blocks(last_entry_point.block(), consumer_block);

				auto consumer_entry_point = function_entry_point.with_depth(last_entry_point.depth())
												.with_block(consumer_block);

				result.push_back({ consumer_entry_point, return_value });
			}
		}

		return result;
	}

	function_call_fragment::function_call_fragment(
		const std::vector<std::string>& names,
		const std::shared_ptr<peg::Ast>& ast
	)
		: names_(names)
		, ast_(ast)
	{
	}

	std::vector<emitter_result> function_call_fragment::build(const emitter& visitor, emitter_context context) const
	{
		auto [score, tail_call_target, target_values] = function_helper::
			try_create_tail_call(context.entryPoint, context.values, begin(names_), end(names_));

		if (score == 0)
		{
			std::vector<const base_value*> cloned;

			std::transform(context.values.begin(), context.values.end(), back_inserter(cloned), [&](auto value) {
				return value->clone(context.entryPoint, clone_strategy::value);
			});

			auto zipped = ranges::views::zip(cloned, target_values);

			auto result_entry_point = std::
				accumulate(zipped.begin(), zipped.end(), context.entryPoint, [&](auto accumulated_entry_point, auto result) {
					auto [value, storage] = result;

					return value_helper::transfer_value(accumulated_entry_point, value, storage, visitor);
				});

			link_blocks(result_entry_point.block(), tail_call_target->block());

			return std::vector<emitter_result>();
		}

		if (score == 1)
		{
			throw std::exception(); // TODO
		}

		return make_function_call(ast_, names_, context);
	}

}
