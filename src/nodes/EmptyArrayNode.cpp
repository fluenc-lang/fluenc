#include "nodes/EmptyArrayNode.h"

EmptyArrayNode::EmptyArrayNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> EmptyArrayNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitEmptyArray(this, entryPoint, values);
}

std::vector<DzResult> EmptyArrayNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitEmptyArray(this, entryPoint, values);
}
