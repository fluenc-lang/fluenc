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

std::vector<DzResult> InstantiationNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitInstantiation(this, entryPoint, values);
}

std::vector<DzResult> InstantiationNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitInstantiation(this, entryPoint, values);
}
