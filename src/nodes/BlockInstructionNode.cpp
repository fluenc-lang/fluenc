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

std::vector<DzResult> BlockInstructionNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}

std::vector<DzResult> BlockInstructionNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}
