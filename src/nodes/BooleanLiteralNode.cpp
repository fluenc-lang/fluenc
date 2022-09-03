#include "nodes/BooleanLiteralNode.h"

BooleanLiteralNode::BooleanLiteralNode(const Node *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

std::vector<DzResult> BooleanLiteralNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitBooleanLiteral(this, context);
}

std::vector<DzResult> BooleanLiteralNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitBooleanLiteral(this, context);
}
