#include "nodes/BinaryNode.h"

BinaryNode::BinaryNode(const Node *consumer, const std::string &op)
	: m_consumer(consumer)
	, m_op(op)
{
}

std::vector<DzResult<BaseValue>> BinaryNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitBinary(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> BinaryNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitBinary(this, entryPoint, values);
}
