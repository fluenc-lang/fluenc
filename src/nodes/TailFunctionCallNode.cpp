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

std::vector<DzResult> TailFunctionCallNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitTailFunctionCall(this, context);
}

std::vector<DzResult> TailFunctionCallNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitTailFunctionCall(this, context);
}
