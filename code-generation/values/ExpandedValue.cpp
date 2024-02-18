#include "EntryPoint.h"

#include "values/ExpandedValue.h"

#include "types/IteratorType.h"

ExpandedValue::ExpandedValue(bool isArray, const Type *iteratorType, const EntryPoint &provider, const fragment *node, const std::vector<const ExpandedValue *> &next, const Stack &values)
	: m_isArray(isArray)
	, m_iteratorType(iteratorType)
	, m_provider(new EntryPoint(provider))
	, m_node(node)
	// , m_chain(chain)
	, m_next(next)
	, m_values(values)
{

}

value_id ExpandedValue::id() const
{
    return value_id::expanded;
}

bool ExpandedValue::isArray() const
{
	return m_isArray;
}

const Type *ExpandedValue::type() const
{
	return m_iteratorType;
}

const BaseValue *ExpandedValue::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}

const EntryPoint *ExpandedValue::provider() const
{
	return m_provider;
}

const fragment* ExpandedValue::node() const
{
	return m_node;
}

// const fragment* ExpandedValue::chain() const
// {
// 	return m_chain;
// }

std::vector<const ExpandedValue *> ExpandedValue::next() const
{
	return m_next;
}

const Stack &ExpandedValue::values() const
{
	return m_values;
}
