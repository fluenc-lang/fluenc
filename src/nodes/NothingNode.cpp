#include "nodes/NothingNode.h"

NothingNode::NothingNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult<BaseValue>> NothingNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitNothing(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> NothingNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitNothing(this, entryPoint, values);
}
