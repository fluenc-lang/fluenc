#include "nodes/IntegralLiteralNode.h"

IntegralLiteralNode::IntegralLiteralNode(const Node *consumer, ITypeName *type, const std::string &value)
	: m_consumer(consumer)
	, m_type(type)
	, m_value(value)
{
}

std::vector<DzResult> IntegralLiteralNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitIntegralLiteral(this, entryPoint, values);
}
