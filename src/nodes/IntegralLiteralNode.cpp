#include "nodes/IntegralLiteralNode.h"

IntegralLiteralNode::IntegralLiteralNode(const Node *consumer, ITypeName *type, const std::string &value)
	: m_consumer(consumer)
	, m_type(type)
	, m_value(value)
{
}

std::vector<DzResult<BaseValue>> IntegralLiteralNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitIntegralLiteral(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> IntegralLiteralNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitIntegralLiteral(this, entryPoint, values);
}
