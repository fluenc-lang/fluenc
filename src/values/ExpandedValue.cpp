#include "EntryPoint.h"

#include "values/ExpandedValue.h"

#include "types/IteratorType.h"

ExpandedValue::ExpandedValue(bool isArray, const Type *iteratorType, const EntryPoint &provider, const Node *node, const Node *chain, const std::vector<const ExpandedValue *> &next)
	: m_isArray(isArray)
	, m_iteratorType(iteratorType)
	, m_provider(new EntryPoint(provider))
	, m_node(node)
	, m_chain(chain)
	, m_next(next)
{

}

bool ExpandedValue::isArray() const
{
	return m_isArray;
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

const Node *ExpandedValue::chain() const
{
	return m_chain;
}

std::vector<const ExpandedValue *> ExpandedValue::next() const
{
	return m_next;
}
