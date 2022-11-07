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
