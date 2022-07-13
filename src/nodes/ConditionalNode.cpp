#include "nodes/ConditionalNode.h"

ConditionalNode::ConditionalNode(const Node *ifFalse, const Node *ifTrue)
	: m_ifTrue(ifTrue)
	, m_ifFalse(ifFalse)
{
}

std::vector<DzResult<BaseValue>> ConditionalNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitConditional(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> ConditionalNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitConditional(this, entryPoint, values);
}
