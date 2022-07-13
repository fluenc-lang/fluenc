#include "nodes/IteratableNode.h"

IteratableNode::IteratableNode(const IIteratable *iteratable)
	: m_iteratable(iteratable)
{
}

std::vector<DzResult<BaseValue>> IteratableNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitIteratable(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> IteratableNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitIteratable(this, entryPoint, values);
}
