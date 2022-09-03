#include "nodes/IteratableNode.h"

IteratableNode::IteratableNode(const IIteratable *iteratable)
	: m_iteratable(iteratable)
{
}

std::vector<DzResult> IteratableNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitIteratable(this, context);
}

std::vector<DzResult> IteratableNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitIteratable(this, context);
}
