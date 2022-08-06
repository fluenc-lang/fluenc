#include "nodes/TailFunctionCallNode.h"

TailFunctionCallNode::TailFunctionCallNode(const std::vector<std::string> &names, const Node *regularCall)
	: m_names(names)
	, m_regularCall(regularCall)
{
}

int TailFunctionCallNode::order(const EntryPoint &entryPoint) const
{
	return m_regularCall->order(entryPoint);
}

std::vector<DzResult> TailFunctionCallNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitTailFunctionCall(this, entryPoint, values);
}

std::vector<DzResult> TailFunctionCallNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitTailFunctionCall(this, entryPoint, values);
}
