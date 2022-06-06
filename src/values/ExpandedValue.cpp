#include "EntryPoint.h"

#include "values/ExpandedValue.h"

#include "types/IteratorType.h"

ExpandedValue::ExpandedValue(const Type *iteratorType, const EntryPoint &provider, const Node *node, const std::vector<const ExpandedValue *> &next)
	: m_iteratorType(iteratorType)
	, m_provider(new EntryPoint(provider))
	, m_node(node)
	, m_next(next)
{

}

const Type *ExpandedValue::type() const
{
	return m_iteratorType;
}

const BaseValue *ExpandedValue::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}

const EntryPoint *ExpandedValue::provider() const
{
	return m_provider;
}

const Node *ExpandedValue::node() const
{
	return m_node;
}

std::vector<const ExpandedValue *> ExpandedValue::next() const
{
	return m_next;
}
