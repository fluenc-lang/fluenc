#include "nodes/FloatLiteralNode.h"

FloatLiteralNode::FloatLiteralNode(const Node *consumer, const ITypeName *type, const std::string &value)
	: m_consumer(consumer)
	, m_type(type)
	, m_value(value)
{
}

std::vector<DzResult> FloatLiteralNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitFloatLiteral(this, context);
}

std::vector<DzResult> FloatLiteralNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitFloatLiteral(this, context);
}
