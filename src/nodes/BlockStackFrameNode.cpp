#include "Utility.h"

#include "nodes/BlockStackFrameNode.h"

BlockStackFrameNode::BlockStackFrameNode(Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult<BaseValue>> BlockStackFrameNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(values);

	return visitor.visitBlockStackFrame(this, entryPoint, Stack<BaseValue>());
}

std::vector<DzResult<BaseValue>> BlockStackFrameNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	UNUSED(values);

	return visitor.visitBlockStackFrame(this, entryPoint, Stack<BaseValue>());
}
