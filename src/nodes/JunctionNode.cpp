#include "nodes/JunctionNode.h"

JunctionNode::JunctionNode(const Node *subject)
	: m_subject(subject)
{
}

std::vector<DzResult> JunctionNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitJunction(this, entryPoint, values);
}
