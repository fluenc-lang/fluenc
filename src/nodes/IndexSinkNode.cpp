#include "nodes/IndexSinkNode.h"

IndexSinkNode::IndexSinkNode(size_t index, const Node *consumer)
	: m_index(index)
	, m_consumer(consumer)
{
}
