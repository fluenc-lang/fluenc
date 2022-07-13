#include "nodes/LocalNode.h"

LocalNode::LocalNode(const Node *consumer, const std::string &name)
	: m_consumer(consumer)
	, m_name(name)
{
}

std::vector<DzResult<BaseValue>> LocalNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitLocal(this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > LocalNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitLocal(this, entryPoint, values);
}
