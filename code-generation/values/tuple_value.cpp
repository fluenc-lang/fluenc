#include "tuple_value.hpp"

#include "types/tuple_type.hpp"

namespace fluenc::code_generation::values
{
	tuple_value::tuple_value(const std::vector<const base_value*>& values)
		: values_(values)
	{
	}

	value_id tuple_value::id() const
	{
		return value_id::tuple;
	}

	size_t tuple_value::size() const
	{
		return values_.size();
	}

	const base_type* tuple_value::type() const
	{
		std::vector<const base_type*> types;

		std::transform(begin(values_), end(values_), std::back_insert_iterator(types), [](auto value) {
			return value->type();
		});

		return types::tuple_type::get(types);
	}

	const base_value* tuple_value::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		std::vector<const base_value*> values;

		std::transform(begin(values_), end(values_), std::back_inserter(values), [&](auto value) {
			return value->clone(entryPoint, strategy);
		});

		return new tuple_value(values);
	}

	value_stack tuple_value::values() const
	{
		return values_;
	}
}
