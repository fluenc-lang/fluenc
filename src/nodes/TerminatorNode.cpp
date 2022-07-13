#include "nodes/TerminatorNode.h"

TerminatorNode::TerminatorNode(const std::string &name, FunctionAttribute attribute)
	: m_name(name)
	, m_attribute(attribute)
{
}

FunctionAttribute TerminatorNode::attribute() const
{
	return m_attribute;
}

TerminatorNode *TerminatorNode::instance()
{
	static TerminatorNode instance("pajs", FunctionAttribute::None);

	return &instance;
}

std::vector<DzResult<BaseValue>> TerminatorNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitTerminator(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> TerminatorNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitTerminator(this, entryPoint, values);
}
