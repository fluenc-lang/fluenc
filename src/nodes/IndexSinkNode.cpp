#include "nodes/IndexSinkNode.h"

IndexSinkNode::IndexSinkNode(size_t index, const Node *consumer)
	: m_index(index)
	, m_consumer(consumer)
{
}

std::vector<DzResult> IndexSinkNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitIndexSink(this, entryPoint, values);
}

std::vector<DzResult> IndexSinkNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitIndexSink(this, entryPoint, values);
}
