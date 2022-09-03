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

std::vector<DzResult> GlobalNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitGlobal(this, context);
}

std::vector<DzResult> GlobalNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitGlobal(this, context);
}
