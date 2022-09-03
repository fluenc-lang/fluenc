#include "nodes/JunctionNode.h"

JunctionNode::JunctionNode(const Node *subject)
	: m_subject(subject)
{
}

std::vector<DzResult> JunctionNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitJunction(this, context);
}

std::vector<DzResult> JunctionNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitJunction(this, context);
}
