#include "nodes/NothingNode.h"

NothingNode::NothingNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> NothingNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitNothing(this, context);
}

std::vector<DzResult> NothingNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitNothing(this, context);
}
