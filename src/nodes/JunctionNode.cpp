#include "nodes/JunctionNode.h"

JunctionNode::JunctionNode(const Node *subject)
	: m_subject(subject)
{
}

std::vector<DzResult<BaseValue>> JunctionNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitJunction(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> JunctionNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitJunction(this, entryPoint, values);
}
