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

std::vector<DzResult<BaseValue>> StackSegmentNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitStackSegment(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> StackSegmentNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitStackSegment(this, entryPoint, values);
}
