#include "EntryPoint.h"
#include "Utility.h"

#include "values/ExpandableValue.h"

#include "types/ExpandedType.h"

ExpandableValue::ExpandableValue(bool isArray
	, const Type *iteratorType
	, const EntryPoint &provider
	, const Node *chain
	, const Stack &values
	)
	: m_isArray(isArray)
	, m_iteratorType(iteratorType)
	, m_provider(new EntryPoint(provider))
	, m_chain(chain)
	, m_values(values)
{
}

ValueId ExpandableValue::id() const
{
	return ValueId::Expandable;
}

bool ExpandableValue::isArray() const
{
	return m_isArray;
}

const Type *ExpandableValue::type() const
{
	return m_iteratorType;
}

const BaseValue *ExpandableValue::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}

const EntryPoint *ExpandableValue::provider() const
{
	return m_provider;
}

const Node *ExpandableValue::chain() const
{
	return m_chain;
}

const ExpandedType *ExpandableValue::expandedType() const
{
	return dynamic_cast<const ExpandedType *>(m_iteratorType);
}

const Stack ExpandableValue::values() const
{
	return m_values;
}
