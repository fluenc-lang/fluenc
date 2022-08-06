#include "Utility.h"

#include "nodes/BlockStackFrameNode.h"

BlockStackFrameNode::BlockStackFrameNode(Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> BlockStackFrameNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	return visitor.visitBlockStackFrame(this, entryPoint, Stack());
}

std::vector<DzResult> BlockStackFrameNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	return visitor.visitBlockStackFrame(this, entryPoint, Stack());
}
