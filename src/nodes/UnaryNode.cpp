#include "nodes/UnaryNode.h"

UnaryNode::UnaryNode(const Node *consumer, const std::string &op)
	: m_consumer(consumer)
	, m_op(op)
{
}
