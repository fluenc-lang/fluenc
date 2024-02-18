#include "ArrayElementNode.h"
#include "Emitter.h"

ArrayElementNode::ArrayElementNode(const std::shared_ptr<peg::Ast> &ast
	, const Type *arrayType
	, const fragment *node
	, const fragment *next
	)
	: m_ast(ast)
	, m_arrayType(arrayType)
	, m_node(node)
	, m_next(next)
{
}

std::vector<DzResult> ArrayElementNode::build(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}
