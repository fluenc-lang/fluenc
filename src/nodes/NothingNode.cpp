#include "nodes/NothingNode.h"

NothingNode::NothingNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> NothingNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitNothing(this, entryPoint, values);
}
