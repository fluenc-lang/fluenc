#include "nodes/FloatLiteralNode.h"

FloatLiteralNode::FloatLiteralNode(const Node *consumer, const ITypeName *type, const std::string &value)
	: m_consumer(consumer)
	, m_type(type)
	, m_value(value)
{
}

std::vector<DzResult> FloatLiteralNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitFloatLiteral(this, entryPoint, values);
}
