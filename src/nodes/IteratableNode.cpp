#include "nodes/IteratableNode.h"

IteratableNode::IteratableNode(const IIteratable *iteratable)
	: m_iteratable(iteratable)
{
}

std::vector<DzResult> IteratableNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitIteratable(this, entryPoint, values);
}

std::vector<DzResult> IteratableNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitIteratable(this, entryPoint, values);
}
