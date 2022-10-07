#include "nodes/UnaryNode.h"

UnaryNode::UnaryNode(const std::shared_ptr<peg::Ast> &ast, const Node *consumer, const std::string &op)
	: m_ast(ast)
	, m_consumer(consumer)
	, m_op(op)
{
}
