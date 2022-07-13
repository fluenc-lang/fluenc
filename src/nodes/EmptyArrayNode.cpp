#include "nodes/EmptyArrayNode.h"

EmptyArrayNode::EmptyArrayNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult<BaseValue>> EmptyArrayNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitEmptyArray(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> EmptyArrayNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitEmptyArray(this, entryPoint, values);
}
