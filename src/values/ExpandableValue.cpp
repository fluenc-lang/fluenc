#include "values/ExpandableValue.h"
#include "EntryPoint.h"

#include "types/ExpandedType.h"

ExpandableValue::ExpandableValue(bool isArray
	, const Type *iteratorType
	, const EntryPoint &provider
	, const Node *chain
	)
	: m_isArray(isArray)
	, m_iteratorType(iteratorType)
	, m_provider(new EntryPoint(provider))
	, m_chain(chain)
{
}

bool ExpandableValue::isArray() const
{
	return m_isArray;
}

const Type *ExpandableValue::type() const
{
	return m_iteratorType;
}

const BaseValue *ExpandableValue::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

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
