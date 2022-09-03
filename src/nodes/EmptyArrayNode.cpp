#include "nodes/EmptyArrayNode.h"

EmptyArrayNode::EmptyArrayNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> EmptyArrayNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitEmptyArray(this, context);
}

std::vector<DzResult> EmptyArrayNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitEmptyArray(this, context);
}
