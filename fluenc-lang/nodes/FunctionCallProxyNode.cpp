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
