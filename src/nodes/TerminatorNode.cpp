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

std::vector<DzResult> TerminatorNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitTerminator(this, context);
}

std::vector<DzResult> TerminatorNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitTerminator(this, context);
}
