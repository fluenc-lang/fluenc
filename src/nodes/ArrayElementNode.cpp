#include "nodes/ArrayElementNode.h"

ArrayElementNode::ArrayElementNode(const Type *arrayType, const Node *node, const Node *next)
	: m_arrayType(arrayType)
	, m_node(node)
	, m_next(next)
{
}

std::vector<DzResult > ArrayElementNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitArrayElement(this, context);
}

std::vector<DzResult > ArrayElementNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitArrayElement(this, context);
}
