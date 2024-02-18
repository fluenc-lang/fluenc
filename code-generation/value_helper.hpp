#pragma once

#include "entry_point.hpp"

#include "values/tuple_value.hpp"

namespace fluenc::code_generation
{
	namespace values
	{
		struct scalar_value;
	}

	struct emitter;
	struct base_value;

	struct value_helper
	{
		static entry_point transfer_value(
			const entry_point& entry_point,
			const base_value* value,
			const base_value* storage,
			const emitter& emitter
		);

		static const values::scalar_value* get_scalar(
			const std::shared_ptr<peg::Ast>& ast,
			const entry_point& entry_point,
			const base_value* value
		);
		static const values::scalar_value* get_scalar(
			const std::shared_ptr<peg::Ast>& ast,
			const entry_point& entryPoint,
			value_stack& values
		);

		template <typename T, typename TContainer>
		static std::vector<const T*> extract_values(TContainer container)
		{
			std::vector<const T*> values;

			for (auto i = container.rbegin(); i != container.rend(); i++)
			{
				if (auto tuple = value_cast<const values::tuple_value*>(*i))
				{
					auto nested = extract_values<T>(tuple->values());

					for (auto& value : nested)
					{
						values.push_back(value);
					}
				}

				if (auto to_find = value_cast<const T*>(*i))
				{
					values.push_back(to_find);
				}
			}

			return values;
		}
	};
}
