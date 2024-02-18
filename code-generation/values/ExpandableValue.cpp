#include "EntryPoint.h"
#include "utility.hpp"

#include "values/ExpandableValue.h"

#include "types/ExpandedType.h"

ExpandableValue::ExpandableValue(bool isArray
	, const Type *iteratorType
	, const EntryPoint &provider
	, const fragment *chain
	, const Stack &values
	)
	: m_isArray(isArray)
	, m_iteratorType(iteratorType)
	, m_provider(new EntryPoint(provider))
	, m_chain(chain)
	, m_values(values)
{
}

value_id ExpandableValue::id() const
{
    return value_id::expandable;
}

bool ExpandableValue::isArray() const
{
	return m_isArray;
}

const Type *ExpandableValue::type() const
{
	return m_iteratorType;
}

const BaseValue *ExpandableValue::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}

const EntryPoint *ExpandableValue::provider() const
{
	return m_provider;
}

const fragment* ExpandableValue::chain() const
{
	return m_chain;
}

const expanded_type *ExpandableValue::expandedType() const
{
    return type_cast<const expanded_type *>(m_iteratorType);
}

const Stack ExpandableValue::values() const
{
	return m_values;
}
