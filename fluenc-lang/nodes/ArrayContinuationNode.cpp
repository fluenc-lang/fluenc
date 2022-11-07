#include "nodes/ArrayContinuationNode.h"

ArrayContinuationNode::ArrayContinuationNode(const ReferenceValue *index, const Node *node, const Type *iteratorType)
	: m_index(index)
	, m_node(node)
	, m_iteratorType(iteratorType)
{
}
