#include "function_helper.hpp"
#include "type.hpp"
#include "type_compatibility_calculator.hpp"

#include "iterators/extremities_iterator.hpp"

namespace fluenc::code_generation
{
	std::tuple<int8_t, const entry_point*> find_entry_point(
		const entry_point& entryPoint,
		const std::string& name,
		const value_stack_t& values
	)
	{
		auto tail_call_candidate = entryPoint.by_name(name);

		if (!tail_call_candidate)
		{
			return { -1, nullptr };
		}

		auto target_entry = tail_call_candidate->entry();
		auto target_values = target_entry->values();

		if (target_values.size() != values.size())
		{
			return find_entry_point(*target_entry->parent(), name, values);
		}

		int8_t min = 0;
		int8_t max = 0;

		std::transform(
			target_values.begin(),
			target_values.end(),
			values.begin(),
			extremities_iterator(min, max),
			[=](auto storage, auto value) {
				auto storageType = storage->type();
				auto valueType = value->type();

				return type_compatibility_calculator::calculate(entryPoint, valueType, storageType);
			}
		);

		if (min < 0 || max > 0)
		{
			auto [score, result] = find_entry_point(*target_entry->parent(), name, values);

			return { std::min(max, score), result };
		}

		return { 0, tail_call_candidate };
	}

	std::tuple<int8_t, const entry_point *, value_stack_t> function_helper::try_create_tail_call(const entry_point& entry_point,
		const value_stack_t &values,
		const std::vector<std::string>::const_iterator& name,
		const std::vector<std::string>::const_iterator& end
	)
	{
		if (name == end)
		{
			return { -1, nullptr, value_stack_t() };
		}

		auto [score, tail_call_candidate] = find_entry_point(entry_point, *name, values);

		if (!tail_call_candidate)
		{
			return try_create_tail_call(entry_point, values, name + 1, end);
		}

		auto target_entry = tail_call_candidate->entry();
		auto target_values = target_entry->values();

		auto tail_call_target = find_tail_call_target(tail_call_candidate, values);

		if (!tail_call_target)
		{
			return try_create_tail_call(entry_point, values, name + 1, end);
		}

		return { score, tail_call_target->entry(), target_values };
	}
}
