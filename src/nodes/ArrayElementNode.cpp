#include "nodes/ArrayElementNode.h"

ArrayElementNode::ArrayElementNode(const Type *arrayType, const Node *node, const Node *next)
	: m_arrayType(arrayType)
	, m_node(node)
	, m_next(next)
{
}

std::vector<DzResult > ArrayElementNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitArrayElement(this, entryPoint, values);
}

std::vector<DzResult > ArrayElementNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitArrayElement(this, entryPoint, values);
}
