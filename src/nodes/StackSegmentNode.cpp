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

std::vector<DzResult> StackSegmentNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitStackSegment(this, entryPoint, values);
}
