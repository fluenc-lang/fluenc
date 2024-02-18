#include "type_compatibility_calculator.hpp"
#include "entry_point.hpp"

#include "ast/struct_node.hpp"

#include "types/aggregate_type.hpp"
#include "types/any_type.hpp"
#include "types/array_type.hpp"
#include "types/buffer_type.hpp"
#include "types/builtin_type.hpp"
#include "types/expanded_type.hpp"
#include "types/function_type.hpp"
#include "types/iterator_type.hpp"
#include "types/opaque_pointer_type.hpp"
#include "types/placeholder_type.hpp"
#include "types/proxy_type.hpp"
#include "types/string_type.hpp"
#include "types/structure_type.hpp"
#include "types/user_type.hpp"

#include "iterators/extremities_iterator.hpp"

#include <numeric>

namespace fluenc::code_generation
{
	struct calculator_storage
	{
		calculator_storage()
		{
			add<types::string_type, types::string_type>([](const entry_point&, auto, auto) {
				return 0;
			});

			add<types::string_type, types::iterator_type>([](const entry_point&, auto, auto) {
				return 1;
			});

			add<types::buffer_type, types::iterator_type>([](const entry_point&, auto, auto) {
				return 1;
			});

			add<types::array_type, types::array_type>([](const entry_point&, auto left, auto right) {
				if (left == right)
				{
					return 0;
				}

				return -1;
			});

			add<types::array_type, types::iterator_type>([](const entry_point&, auto, auto) {
				return 0;
			});

			add<types::user_type,
				types::user_type>([](const entry_point& entry_point, const auto left, auto right) -> int8_t {
				auto left_element_types = left->element_types();
				auto right_element_types = right->element_types();

				if (left_element_types.size() != right_element_types.size())
				{
					return -1;
				}

				auto compatibility = type_compatibility_calculator::
					calculate(entry_point, left->prototype(), right->prototype());

				if (compatibility < 0)
				{
					return compatibility;
				}

				int8_t min = 0;
				int8_t max = 0;

				std::transform(
					begin(left_element_types),
					end(left_element_types),
					begin(right_element_types),
					extremities_iterator(min, max),
					[&](auto left, auto right) {
						return type_compatibility_calculator::calculate(entry_point, left, right);
					}
				);

				if (min < 0 || max > 0)
				{
					return compatibility;
				}

				return 0;
			});

			add<types::user_type, base_type>([](const entry_point& entryPoint, const auto left, auto right) {
				return type_compatibility_calculator::calculate(entryPoint, left->prototype(), right);
			});

			add<types::tuple_type, types::tuple_type>(
				[](const entry_point& entry_point, const auto left, auto right) -> int8_t {
					if (left == right)
					{
						return 0;
					}

					auto left_types = left->types();
					auto right_types = right->types();

					if (left_types.size() != right_types.size())
					{
						return -1;
					}

					int8_t min = 0;
					int8_t max = 0;

					std::transform(
						begin(left_types),
						end(left_types),
						begin(right_types),
						extremities_iterator(min, max),
						[&](auto left, auto right) {
							return type_compatibility_calculator::calculate(entry_point, left, right);
						}
					);

					if (min < 0)
					{
						return min;
					}

					return max;
				}
			);

			add<types::proxy_type, base_type>([](const entry_point&, const auto, auto) {
				return -1;
			});

			add<types::placeholder_type, base_type>([](const entry_point&, const auto, auto) {
				return 0;
			});

			add<types::opaque_pointer_type, types::opaque_pointer_type>(
				[](const entry_point& entry_point, const auto left, auto right) -> int8_t {
					if (left->subject() == right->subject())
					{
						return 0;
					}

					return type_compatibility_calculator::calculate(entry_point, left->subject(), right->subject());
				}
			);

			add<types::opaque_pointer_type, base_type>(
				[](const entry_point& entry_point, const auto left, auto right) -> int8_t {
					if (left->subject() == right)
					{
						return 0;
					}

					return type_compatibility_calculator::calculate(entry_point, left->subject(), right);
				}
			);

			add<types::iterator_type, types::iterator_type>([](const entry_point&, const auto left, auto right) {
				if (left == right)
				{
					return 0;
				}

				return 1;
			});

			add<types::structure_type, types::any_type>([](const entry_point& entryPoint, auto left, auto) -> int8_t {
				auto solve =
					[&](int8_t score, const types::structure_type* type, const entry_point& entry_point, auto solve
					) -> int8_t {
					auto prototype = type->subject();

					return std::accumulate(
						begin(prototype->parent_types),
						end(prototype->parent_types),
						score,
						[=](auto accumulated, auto parent_type) -> int8_t {
							auto resolved_type = type_for(parent_type, entry_point);
							auto prototype = static_cast<const types::structure_type*>(resolved_type);

							return std::max(accumulated, solve(score + 1, prototype, entry_point, solve));
						}
					);
				};

				return solve(2, left, entryPoint, solve);
			});

			add<types::structure_type, base_type>([](const entry_point& entryPoint, auto left, auto right) -> int8_t {
				auto solve =
					[&](int8_t score, const types::structure_type* type, const entry_point& entry_point, auto solve
					) -> std::pair<bool, int8_t> {
					if (right->name() == type->name())
					{
						return { true, score };
					}

					auto prototype = type->subject();

					return std::accumulate(
						begin(prototype->parent_types),
						end(prototype->parent_types),
						std::make_pair(false, score),
						[=](auto accumulated, auto parentType) -> std::pair<bool, int8_t> {
							auto resolved_type = type_for(parentType, entry_point);
							auto prototype = static_cast<const types::structure_type*>(resolved_type);

							auto [accumulated_match, accumulated_score] = accumulated;
							auto [current_match, current_score] = solve(score + 1, prototype, entry_point, solve);

							if (!accumulated_match)
							{
								return { current_match, current_score };
							}

							if (!current_match)
							{
								return { accumulated_match, accumulated_score };
							}

							return { true, std::min(current_score, accumulated_score) };
						}
					);
				};

				auto [match, score] = solve(1, left, entryPoint, solve);

				if (match)
				{
					return score;
				}

				return -1;
			});

			add<types::function_type, types::function_type>(
				[](const entry_point& entry_point, auto left, auto right) -> int8_t {
					if (left == right)
					{
						return 0;
					}

					auto left_types = left->types();
					auto right_types = right->types();

					if (left_types.size() != right_types.size())
					{
						return -1;
					}

					int8_t min = 0;
					int8_t max = 0;

					std::transform(
						begin(left_types),
						end(left_types),
						begin(right_types),
						extremities_iterator(min, max),
						[&](auto left, auto right) {
							return type_compatibility_calculator::calculate(entry_point, left, right);
						}
					);

					if (min < 0)
					{
						return min;
					}

					if (left->function() != right->function())
					{
						return std::max(static_cast<int8_t>(1), max);
					}

					return max;
				}
			);

			add<types::expanded_type, types::expanded_type>(
				[](const entry_point& entry_point, auto left, auto right) -> int8_t {
					if (left == right)
					{
						return 0;
					}

					auto left_types = left->types();
					auto right_types = right->types();

					if (left_types.size() != right_types.size())
					{
						return -1;
					}

					int8_t min = 0;
					int8_t max = 0;

					std::transform(
						begin(left_types),
						end(left_types),
						begin(right_types),
						extremities_iterator(min, max),
						[&](auto left, auto right) {
							return type_compatibility_calculator::calculate(entry_point, left, right);
						}
					);

					if (min < 0)
					{
						return min;
					}

					return max;
				}
			);

			add<types::expanded_type, types::iterator_type>([](const entry_point&, auto, auto) {
				return 1;
			});

			add<types::aggregate_type, base_type>([](const entry_point& entry_point, auto left, auto right) -> int8_t {
				if (left == right)
				{
					return 0;
				}

				auto subjects = left->subjects();

				return std::accumulate(begin(subjects), end(subjects), static_cast<int8_t>(-1), [&](auto score, auto subject) {
					auto result = type_compatibility_calculator::calculate(entry_point, subject, right);

					if (result < 0)
					{
						return score;
					}

					if (score < 0)
					{
						return result;
					}

					return std::min(result, score);
				});
			});

			add<types::any_type, types::builtin_base_type>([](const entry_point&, auto, auto) {
				return 1;
			});
		}

		template <typename TLeft, typename TRight>
		void add(type_compatibility_calculator::calculator_t<TLeft, TRight> calculator)
		{
			auto strategy = new type_compatibility_calculator::strategy<TLeft, TRight>(calculator);

			calculators.push_back(strategy);
		}

		std::vector<type_compatibility_calculator::strategy_base*> calculators;
	};

	int8_t type_compatibility_calculator::calculate(
		const entry_point& entry_point,
		const base_type* left,
		const base_type* right
	)
	{
		static calculator_storage storage;

		for (auto& strategy : storage.calculators)
		{
			if (auto result = strategy->calculate(entry_point, left, right))
			{
				return *result;
			}
		}

		if (left == right)
		{
			return 0;
		}

		if (left->name() == right->name())
		{
			return 1;
		}

		return -1;
	}
}
