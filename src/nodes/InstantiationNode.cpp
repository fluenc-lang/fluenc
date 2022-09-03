#include "nodes/InstantiationNode.h"

InstantiationNode::InstantiationNode(const Node *consumer
	, const IPrototypeProvider *prototypeProvider
	, const std::shared_ptr<peg::Ast> &ast
	, const std::vector<std::string> &fields
	)
	: m_consumer(consumer)
	, m_prototypeProvider(prototypeProvider)
	, m_ast(ast)
	, m_fields(fields)
{
}

std::vector<DzResult> InstantiationNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitInstantiation(this, context);
}

std::vector<DzResult> InstantiationNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitInstantiation(this, context);
}
