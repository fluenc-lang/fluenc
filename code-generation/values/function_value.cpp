#include "entry_point.hpp"
#include "utility.hpp"

#include "values/function_value.hpp"

#include "types/aggregate_type.hpp"
#include "types/function_type.hpp"

#include "ast/function_node.hpp"

namespace fluenc::code_generation::values
{
	function_value::function_value(const std::vector<function_node*> functions, const entry_point& entryPoint)
		: functions_(functions)
		, entry_point_(new entry_point(entryPoint))
	{
	}

	value_id function_value::id() const
	{
		return value_id::function;
	}

	const base_type* function_value::type() const
	{
		std::vector<const base_type*> function_types;

		std::transform(begin(functions_), end(functions_), back_inserter(function_types), [this](auto function) {
			std::vector<const base_type*> argument_types;

			std::transform(
				begin(function->signature.arguments),
				end(function->signature.arguments),
				back_inserter(argument_types),
				[this](auto argument) {
					return arg_type(argument, *entry_point_);
				}
			);

			return types::function_type::get(argument_types, function);
		});

		return types::aggregate_type::get(function_types);
	}

	const base_value* function_value::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		UNUSED(entryPoint);
		UNUSED(strategy);

		return this;
	}

	std::vector<function_node*> function_value::functions() const
	{
		return functions_;
	}
}
