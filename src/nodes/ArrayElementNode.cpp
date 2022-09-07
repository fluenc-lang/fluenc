#include "nodes/ArrayElementNode.h"

ArrayElementNode::ArrayElementNode(const Type *arrayType, const Node *node, const Node *next)
	: m_arrayType(arrayType)
	, m_node(node)
	, m_next(next)
{
}
