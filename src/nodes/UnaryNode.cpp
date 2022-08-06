#include "nodes/UnaryNode.h"

UnaryNode::UnaryNode(const Node *consumer, const std::string &op)
	: m_consumer(consumer)
	, m_op(op)
{
}

std::vector<DzResult> UnaryNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitUnary(this, entryPoint, values);
}

std::vector<DzResult> UnaryNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitUnary(this, entryPoint, values);
}
