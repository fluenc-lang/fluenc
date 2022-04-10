#include "nodes/BlockInstructionNode.h"

BlockInstructionNode::BlockInstructionNode(const Node *subject, bool containsIterator)
	: m_subject(subject)
	, m_containsIterator(containsIterator)
{
}

bool BlockInstructionNode::containsIterator() const
{
	return m_containsIterator;
}

std::vector<DzResult> BlockInstructionNode::build(const EntryPoint &entryPoint, Stack values) const
{
	return m_subject->build(entryPoint, values);
}
