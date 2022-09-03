#include "nodes/ConditionalNode.h"

ConditionalNode::ConditionalNode(const Node *ifFalse, const Node *ifTrue)
	: m_ifTrue(ifTrue)
	, m_ifFalse(ifFalse)
{
}

std::vector<DzResult> ConditionalNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitConditional(this, context);
}

std::vector<DzResult> ConditionalNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitConditional(this, context);
}
