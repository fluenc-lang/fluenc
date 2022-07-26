#include "nodes/BooleanLiteralNode.h"

BooleanLiteralNode::BooleanLiteralNode(const Node *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

std::vector<DzResult> BooleanLiteralNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitBooleanLiteral(this, entryPoint, values);
}
