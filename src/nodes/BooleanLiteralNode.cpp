#include "nodes/BooleanLiteralNode.h"

BooleanLiteralNode::BooleanLiteralNode(const Node *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}
