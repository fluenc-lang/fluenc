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

std::vector<DzResult<BaseValue>> MemberAccessNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitMemberAccess(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> MemberAccessNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitMemberAccess(this, entryPoint, values);
}
