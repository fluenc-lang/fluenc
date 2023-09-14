#include "nodes/GlobalNode.h"

GlobalNode::GlobalNode(const std::shared_ptr<peg::Ast> &ast, Node *value, const std::string &name)
	: m_ast(ast)
	, m_value(value)
	, m_name(name)
{
}

const std::shared_ptr<peg::Ast> GlobalNode::ast() const
{
	return m_ast;
}

std::string GlobalNode::name() const
{
	return m_name;
}
