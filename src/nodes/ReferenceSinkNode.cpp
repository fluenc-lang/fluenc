#include "nodes/ReferenceSinkNode.h"

ReferenceSinkNode::ReferenceSinkNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult<BaseValue>> ReferenceSinkNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitReferenceSink(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> ReferenceSinkNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitReferenceSink(this, entryPoint, values);
}
