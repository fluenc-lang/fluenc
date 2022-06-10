#include "nodes/IteratableNode.h"

#include "values/IIteratable.h"

IteratableNode::IteratableNode(const IIteratable *iteratable)
	: m_iteratable(iteratable)
{
}

std::vector<DzResult> IteratableNode::build(const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	return m_iteratable->build(entryPoint);
}
