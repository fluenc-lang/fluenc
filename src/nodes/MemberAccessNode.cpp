#include "nodes/MemberAccessNode.h"

MemberAccessNode::MemberAccessNode(const Node *consumer
	, const std::shared_ptr<peg::Ast> &ast
	, const std::vector<std::string> &names
	)
	: m_consumer(consumer)
	, m_ast(ast)
	, m_names(names)
{
}

std::vector<DzResult> MemberAccessNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitMemberAccess(this, context);
}

std::vector<DzResult> MemberAccessNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitMemberAccess(this, context);
}
