#include "nodes/IntegralLiteralNode.h"

IntegralLiteralNode::IntegralLiteralNode(const Node *consumer, ITypeName *type, const std::string &value)
	: m_consumer(consumer)
	, m_type(type)
	, m_value(value)
{
}
