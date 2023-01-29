#include "nodes/ConditionalNode.h"

ConditionalNode::ConditionalNode(const std::shared_ptr<peg::Ast> &ast, const Node *ifFalse, const Node *ifTrue)
	: m_ast(ast)
	, m_ifTrue(ifTrue)
	, m_ifFalse(ifFalse)
{
}
