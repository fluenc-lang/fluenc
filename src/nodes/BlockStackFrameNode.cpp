#include "nodes/BlockStackFrameNode.h"

BlockStackFrameNode::BlockStackFrameNode(Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> BlockStackFrameNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitBlockStackFrame(this, { context.entryPoint, Stack() });
}

std::vector<DzResult> BlockStackFrameNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitBlockStackFrame(this, { context.entryPoint, Stack() });
}
