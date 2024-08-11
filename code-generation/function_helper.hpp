#pragma once

#include <numeric>
#include <string>
#include <tuple>
#include <vector>

#include "entry_point.hpp"

#include "values/expanded_value.hpp"
#include "values/tuple_value.hpp"
#include "values/user_type_value.hpp"

namespace fluenc::code_generation
{
	class value_stack;

	struct function_helper
	{
		static std::tuple<int8_t, const entry_point*, value_stack> try_create_tail_call(
			const entry_point& entry_point,
			const value_stack& values,
			const std::vector<std::string>::const_iterator& name,
			const std::vector<std::string>::const_iterator& end
		);

		template <typename Container>
		static const entry_point* find_tail_call_target(const entry_point* candidate, Container container)
		{
			return std::accumulate(
				container.begin(),
				container.end(),
				candidate,
				[&](const entry_point* target, auto value) -> const entry_point* {
					if (auto user_type = value_cast<const values::user_type_value*>(value))
					{
						auto fields = user_type->fields();

						std::vector<const base_value*> values;

						std::transform(begin(fields), end(fields), std::back_inserter(values), [](auto field) {
							return field->value();
						});

						return find_tail_call_target(target, values);
					}

					if (auto tuple = value_cast<const values::tuple_value*>(value))
					{
						return find_tail_call_target(target, tuple->values());
					}

					if (auto expanded = value_cast<const values::expanded_value*>(value))
					{
						auto provider = find_tail_call_target(expanded->provider(), expanded->next());

						if (provider->depth() < target->depth())
						{
							return provider;
						}
					}

					return target;
				}
			);
		}
	};
}
