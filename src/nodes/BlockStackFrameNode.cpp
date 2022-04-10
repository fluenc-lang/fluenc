#include "nodes/BlockStackFrameNode.h"

BlockStackFrameNode::BlockStackFrameNode(Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> BlockStackFrameNode::build(const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	return m_consumer->build(entryPoint, Stack());
}
