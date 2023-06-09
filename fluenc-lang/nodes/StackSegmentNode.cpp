#include "nodes/StackSegmentNode.h"

StackSegmentNode::StackSegmentNode(std::vector<Node *> values, const Node *call, const Node *consumer)
	: m_values(values)
	, m_call(call)
	, m_consumer(consumer)
{
}

int StackSegmentNode::order(const EntryPoint &entryPoint) const
{
	return m_call->order(entryPoint);
}
