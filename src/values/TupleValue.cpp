#include "values/TupleValue.h"

TupleValue::TupleValue(const Type *iteratorType, const std::vector<const BaseValue *> &values)
	: m_iteratorType(iteratorType)
	, m_values(values)
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

	return TupleType::get(m_iteratorType, types);
}

const Type *TupleValue::iteratorType() const
{
	return m_iteratorType;
}

const BaseValue *TupleValue::clone(const EntryPoint &entryPoint) const
{
	std::vector<const BaseValue *> values;

	std::transform(begin(m_values), end(m_values), std::back_inserter(values), [&](auto value)
	{
		return value->clone(entryPoint);
	});

	return new TupleValue(m_iteratorType, values);
}

Stack TupleValue::values() const
{
	return m_values;
}
