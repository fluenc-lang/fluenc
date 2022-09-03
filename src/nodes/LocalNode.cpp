#include "nodes/LocalNode.h"

LocalNode::LocalNode(const Node *consumer, const std::string &name)
	: m_consumer(consumer)
	, m_name(name)
{
}

std::vector<DzResult> LocalNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitLocal(this, context);
}

std::vector<DzResult > LocalNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitLocal(this, context);
}
