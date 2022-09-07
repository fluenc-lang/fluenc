#include "nodes/ArraySinkNode.h"

ArraySinkNode::ArraySinkNode(size_t size
	, const Node *consumer
	, const Node *firstValue
	)
	: m_size(size)
	, m_consumer(consumer)
	, m_firstValue(firstValue)
{
}
