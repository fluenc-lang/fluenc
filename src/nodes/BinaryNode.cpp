#include "nodes/BinaryNode.h"

BinaryNode::BinaryNode(const Node *consumer, const std::string &op)
	: m_consumer(consumer)
	, m_op(op)
{
}

std::vector<DzResult> BinaryNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitBinary(this, context);
}

std::vector<DzResult> BinaryNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitBinary(this, context);
}
