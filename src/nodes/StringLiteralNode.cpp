#include "nodes/StringLiteralNode.h"

StringLiteralNode::StringLiteralNode(const Node *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

std::vector<DzResult> StringLiteralNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitStringLiteral(this, entryPoint, values);
}

std::vector<DzResult> StringLiteralNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitStringLiteral(this, entryPoint, values);
}
