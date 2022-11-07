#include "values/TupleValue.h"

#include "types/TupleType.h"

TupleValue::TupleValue(const std::vector<const BaseValue *> &values)
	: m_values(values)
{
}

size_t TupleValue::size() const
{
	return m_values.size();
}

const Type *TupleValue::type() const
{
	std::vector<const Type *> types;

	std::transform(begin(m_values), end(m_values), std::back_insert_iterator(types), [](auto value)
	{
		return value->type();
	});

	return TupleType::get(types);
}

const BaseValue *TupleValue::clone(const EntryPoint &entryPoint) const
{
	std::vector<const BaseValue *> values;

	std::transform(begin(m_values), end(m_values), std::back_inserter(values), [&](auto value)
	{
		return value->clone(entryPoint);
	});

	return new TupleValue(values);
}

Stack TupleValue::values() const
{
	return m_values;
}
