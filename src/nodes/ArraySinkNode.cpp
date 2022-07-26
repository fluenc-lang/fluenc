#include "nodes/ArraySinkNode.h"

ArraySinkNode::ArraySinkNode(size_t size
	, const Node *consumer
	, const Node *firstValue
	)
	: m_size(size)
	, m_consumer(consumer)
	, m_firstValue(firstValue)
{
}

std::vector<DzResult> ArraySinkNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitArraySink(this, entryPoint, values);
}
