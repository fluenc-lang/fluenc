#include "nodes/GlobalNode.h"

GlobalNode::GlobalNode(Node *value, const std::string &name)
	: m_value(value)
	, m_name(name)
{
}

std::string GlobalNode::name() const
{
	return m_name;
}

std::vector<DzResult<BaseValue>> GlobalNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitGlobal(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> GlobalNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitGlobal(this, entryPoint, values);
}
