#include "nodes/ReferenceSinkNode.h"

ReferenceSinkNode::ReferenceSinkNode(const Node *consumer)
	: m_consumer(consumer)
{
}
