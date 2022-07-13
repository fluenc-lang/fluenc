#include "nodes/IndexSinkNode.h"

IndexSinkNode::IndexSinkNode(size_t index, const Node *consumer)
	: m_index(index)
	, m_consumer(consumer)
{
}

std::vector<DzResult<BaseValue>> IndexSinkNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitIndexSink(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> IndexSinkNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitIndexSink(this, entryPoint, values);
}
