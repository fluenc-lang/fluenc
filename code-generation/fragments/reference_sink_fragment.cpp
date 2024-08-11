#include "reference_sink_fragment.hpp"
#include "emitter.hpp"
#include "ir_builder.hpp"

#include "values/reference_value.hpp"
#include "values/scalar_value.hpp"
#include "values/tuple_value.hpp"
#include "values/user_type_value.hpp"

namespace fluenc::code_generation::fragments
{
	reference_sink_fragment::reference_sink_fragment(const fragment* consumer)
		: consumer_(consumer)
	{
	}

	const base_value* make_reference(const base_value* value, const entry_point& entry_point)
	{
		ir_builder builder(entry_point);

		if (auto typed_value = value_cast<const values::scalar_value*>(value))
		{
			auto argumentType = typed_value->type();

			auto alloc = entry_point.alloc(argumentType);

			builder.create_store(typed_value, alloc);

			return alloc;
		}

		if (auto reference_value = value_cast<const values::reference_value*>(value))
		{
			return reference_value->clone(entry_point, clone_strategy::value);
		}

		if (auto tuple_value = value_cast<const values::tuple_value*>(value))
		{
			auto tuple_elements = tuple_value->values();

			std::vector<const base_value*> values;

			std::transform(tuple_elements.begin(), tuple_elements.end(), std::back_inserter(values), [&](auto value) {
				return make_reference(value, entry_point);
			});

			return new values::tuple_value(values);
		}

		if (auto user_type_value = value_cast<const values::user_type_value*>(value))
		{
			auto fields = user_type_value->fields();

			std::vector<const values::named_value*> values;

			std::transform(begin(fields), end(fields), std::back_inserter(values), [&](auto field) {
				return new values::named_value { field->name(), make_reference(field->value(), entry_point) };
			});

			return new values::user_type_value { user_type_value->prototype(), values };
		}

		return value;
	};

	std::vector<emitter_result> reference_sink_fragment::build(const emitter& visitor, emitter_context context) const
	{
		auto value = make_reference(context.values.pop(), context.entryPoint);

		context.values.push(value);

		return consumer_->build(visitor, context);
	}
}
