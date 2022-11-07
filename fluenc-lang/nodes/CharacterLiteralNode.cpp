#include "nodes/CharacterLiteralNode.h"

CharacterLiteralNode::CharacterLiteralNode(const Node *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}
