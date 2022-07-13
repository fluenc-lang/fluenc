#include "nodes/BooleanLiteralNode.h"

BooleanLiteralNode::BooleanLiteralNode(const Node *consumer, const std::string &value)
	: m_consumer(consumer)
	, m_value(value)
{
}

std::vector<DzResult<BaseValue>> BooleanLiteralNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitBooleanLiteral(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> BooleanLiteralNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitBooleanLiteral(this, entryPoint, values);
}
