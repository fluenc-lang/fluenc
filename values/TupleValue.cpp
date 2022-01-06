#include "TupleValue.h"

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

Stack TupleValue::values() const
{
	return m_values;
}
