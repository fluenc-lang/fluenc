#include "nodes/ArrayElementNode.h"

ArrayElementNode::ArrayElementNode(const std::shared_ptr<peg::Ast> &ast
	, const Type *arrayType
	, const Node *node
	, const Node *next
	)
	: m_ast(ast)
	, m_arrayType(arrayType)
	, m_node(node)
	, m_next(next)
{
}
