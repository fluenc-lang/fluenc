#include "ExpandableValue.h"
#include "EntryPoint.h"

#include "types/IteratorType.h"

ExpandableValue::ExpandableValue(const Type *iteratorType
	, const EntryPoint &provider
	, const Node *chain
	)
	: m_iteratorType(iteratorType)
	, m_provider(new EntryPoint(provider))
	, m_chain(chain)
{
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
