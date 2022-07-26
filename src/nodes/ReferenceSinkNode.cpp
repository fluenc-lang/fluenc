#include "nodes/ReferenceSinkNode.h"

ReferenceSinkNode::ReferenceSinkNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> ReferenceSinkNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitReferenceSink(this, entryPoint, values);
}
