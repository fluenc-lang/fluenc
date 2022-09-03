#include "nodes/UnaryNode.h"

UnaryNode::UnaryNode(const Node *consumer, const std::string &op)
	: m_consumer(consumer)
	, m_op(op)
{
}

std::vector<DzResult> UnaryNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitUnary(this, context);
}

std::vector<DzResult> UnaryNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitUnary(this, context);
}
