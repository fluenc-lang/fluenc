#include "nodes/LazySinkNode.h"

LazySinkNode::LazySinkNode(const Type *iteratorType
	, const Node *consumer
	, const Node *subject
	)
	: m_iteratorType(iteratorType)
	, m_consumer(consumer)
	, m_subject(subject)
{
}
