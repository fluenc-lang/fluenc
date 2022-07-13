#include "nodes/FunctionCallProxyNode.h"

FunctionCallProxyNode::FunctionCallProxyNode(const std::vector<std::string> &names
	, const Node *regularCall
	, const Node *consumer
	)
	: m_names(names)
	, m_regularCall(regularCall)
	, m_consumer(consumer)
{
}

std::vector<DzResult<BaseValue>> FunctionCallProxyNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitFunctionCallProxy(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> FunctionCallProxyNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitFunctionCallProxy(this, entryPoint, values);
}
