#include "values/TupleValue.h"

#include "types/TupleType.h"

tuple_value::tuple_value(const std::vector<const BaseValue *> &values)
	: m_values(values)
{
}

value_id tuple_value::id() const
{
    return value_id::tuple;
}

size_t tuple_value::size() const
{
	return m_values.size();
}

const Type *tuple_value::type() const
{
	std::vector<const Type *> types;

	std::transform(begin(m_values), end(m_values), std::back_insert_iterator(types), [](auto value)
	{
		return value->type();
	});

        return tuple_type::get(types);
}

const BaseValue *tuple_value::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
	std::vector<const BaseValue *> values;

	std::transform(begin(m_values), end(m_values), std::back_inserter(values), [&](auto value)
	{
		return value->clone(entryPoint, strategy);
	});

        return new tuple_value(values);
}

Stack tuple_value::values() const
{
	return m_values;
}
