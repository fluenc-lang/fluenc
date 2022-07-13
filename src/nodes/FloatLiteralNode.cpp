#include "nodes/FloatLiteralNode.h"

FloatLiteralNode::FloatLiteralNode(const Node *consumer, const ITypeName *type, const std::string &value)
	: m_consumer(consumer)
	, m_type(type)
	, m_value(value)
{
}

std::vector<DzResult<BaseValue>> FloatLiteralNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitFloatLiteral(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> FloatLiteralNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitFloatLiteral(this, entryPoint, values);
}
