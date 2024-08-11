#pragma once

#include <map>
#include <optional>

#include "type.hpp"

namespace fluenc::code_generation
{
	struct type_compatibility_calculator
	{
		template <typename TLeft, typename TRight>
		using calculator_t = std::function<int8_t(const entry_point&, const TLeft*, const TRight*)>;

		struct strategy_base
		{
			virtual std::optional<int8_t> calculate(
				const entry_point& entry_point,
				const base_type* left,
				const base_type* right
			) const = 0;
		};

		template <typename TLeft, typename TRight>
		struct strategy : public strategy_base
		{
			strategy(calculator_t<TLeft, TRight> callback)
				: callback_(callback)
			{
			}

			std::optional<int8_t> calculate(const entry_point& entry_point, const base_type* left, const base_type* right)
				const override
			{
				std::vector<const base_type*> types = { left, right };

				for (auto i = 0; i < 2; i++)
				{
					auto casted_left = type_cast<const TLeft*>(types[0]);
					auto casted_right = type_cast<const TRight*>(types[1]);

					if (casted_left && casted_right)
					{
						return callback_(entry_point, casted_left, casted_right);
					}

					std::reverse(begin(types), end(types));
				}

				return {};
			}

		private:
			calculator_t<TLeft, TRight> callback_;
		};

		static int8_t calculate(const entry_point& entry_point, const base_type* left, const base_type* right);
	};
}
