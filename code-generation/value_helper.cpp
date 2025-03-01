#include <numeric>

#include <range/v3/view.hpp>

#include "ast.hpp"
#include "ir_builder.hpp"
#include "transfer_strategy.hpp"
#include "utility.hpp"
#include "value_helper.hpp"

#include "values/buffer_value.hpp"
#include "values/expandable_value.hpp"
#include "values/expanded_value.hpp"
#include "values/pre_lazy_value.hpp"
#include "values/reference_value.hpp"
#include "values/scalar_value.hpp"
#include "values/string_value.hpp"
#include "values/tuple_value.hpp"
#include "values/user_type_value.hpp"

namespace fluenc::code_generation
{
	entry_point value_helper::transfer_value(
		const entry_point& entryPoint,
		const base_value* value,
		const base_value* storage,
		const emitter& emitter
	)
	{
		static std::vector<const transfer_strategy_base*> strategies = {
			strategy_for<values::scalar_value, values::reference_value>(
				[](auto entry_point, auto source, auto target, auto& emitter) {
					UNUSED(emitter);

					ir_builder builder(entry_point);

					builder.create_store(source, target);

					return entry_point;
				}
			),
			strategy_for<values::reference_value, values::reference_value>(
				[](auto entry_point, auto source, auto target, auto& emitter) {
					UNUSED(emitter);

					ir_builder builder(entry_point);

					auto load = builder.create_load(source);

					builder.create_store(load, target);

					return entry_point;
				}
			),
			strategy_for<values::pre_lazy_value, values::pre_lazy_value>(
				[](auto entry_point, auto source, auto target, auto& emitter) {
					return target->assign_from(entry_point, source, emitter);
				}
			),
			strategy_for<base_value, values::pre_lazy_value>([](auto entry_point, auto source, auto target, auto& emitter) {
				return target->assign_from(entry_point, source, emitter);
			}),
			strategy_for<values::user_type_value, values::user_type_value>([](auto entry_point,
																			  auto source,
																			  auto target,
																			  auto& emitter) {
				auto value_fields = source->fields();
				auto storage_fields = target->fields();

				auto zipped = ranges::views::zip(value_fields, storage_fields);

				return std::accumulate(zipped.begin(), zipped.end(), entry_point, [&](auto accumulated_entry_point, auto fields) {
					auto [valueField, storageField] = fields;

					return transfer_value(accumulated_entry_point, valueField->value(), storageField->value(), emitter);
				});
			}),
			strategy_for<values::tuple_value, values::tuple_value>([](auto entry_point,
																	  auto source,
																	  auto target,
																	  auto& emitter) {
				auto value_elements = source->values();
				auto storage_elements = target->values();

				auto zipped = ranges::views::zip(value_elements, storage_elements);

				return std::
					accumulate(zipped.begin(), zipped.end(), entry_point, [&](auto accumulated_entry_point, auto elements) {
						auto [value_element, storage_element] = elements;

						return transfer_value(accumulated_entry_point, value_element, storage_element, emitter);
					});
			}),
			strategy_for<values::string_value, values::string_value>(
				[](auto entry_point, auto source, auto target, auto& emitter) {
					UNUSED(emitter);

					ir_builder builder(entry_point);

					auto reference_load = builder.create_load(source->reference());
					auto length_load = builder.create_load(source->length());

					builder.create_store(reference_load, target->reference());
					builder.create_store(length_load, target->length());

					return entry_point;
				}
			),
			strategy_for<values::expanded_value, values::expandable_value>([](auto entry_point,
																			  auto source,
																			  auto target,
																			  auto& emitter) {
				auto source_values = source->values();
				auto target_values = target->values();

				auto zipped = ranges::views::zip(source_values, target_values);

				return std::
					accumulate(zipped.begin(), zipped.end(), entry_point, [&](auto accumulated_entry_point, auto elements) {
						auto [source_value, target_value] = elements;

						return transfer_value(accumulated_entry_point, source_value, target_value, emitter);
					});
			}),
			strategy_for<values::buffer_value, values::buffer_value>(
				[](auto entry_point, auto source, auto target, auto& emitter) {
					UNUSED(emitter);

					ir_builder builder(entry_point);

					auto reference_load = builder.create_load(source->address());

					builder.create_store(reference_load, target->address());

					return entry_point;
				}
			),
		};

		auto result_entry_point = std::
			accumulate(begin(strategies), end(strategies), std::optional<entry_point>(), [&](auto accumulated, auto strategy) {
				if (accumulated)
				{
					return accumulated;
				}

				return strategy->invoke(entryPoint, value, storage, emitter);
			});

		return result_entry_point.value_or(entryPoint);
	}

	const values::scalar_value* value_helper::get_scalar(
		const std::shared_ptr<peg::Ast>& ast,
		const entry_point& entry_point,
		const base_value* value
	)
	{
		if (auto scalar = value_cast<const values::scalar_value*>(value))
		{
			return scalar;
		}

		if (auto reference = value_cast<const values::reference_value*>(value))
		{
			ir_builder builder(entry_point);

			return builder.create_load(reference);
		}

		auto& expected_metadata = values::scalar_value::static_metadata();
		auto& actual_metadata = value->metadata();

		throw invalid_type_exception(ast, expected_metadata.name(), actual_metadata.name());
	}

	const values::scalar_value* value_helper::get_scalar(const std::shared_ptr<peg::Ast>& ast,
		const entry_point& entryPoint,
		value_stack_t &values
	)
	{
		auto value = values.pop();

		return get_scalar(ast, entryPoint, value);
	}
}
