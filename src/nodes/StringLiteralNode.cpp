#include "nodes/StringLiteralNode.h"

StringLiteralNode::StringLiteralNode(const Node *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

std::vector<DzResult> StringLiteralNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitStringLiteral(this, context);
}

std::vector<DzResult> StringLiteralNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitStringLiteral(this, context);
}
