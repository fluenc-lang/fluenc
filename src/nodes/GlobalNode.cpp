#include <llvm/IR/Constants.h>

#include "nodes/GlobalNode.h"
#include "Type.h"

GlobalNode::GlobalNode(Node *value, const std::string &name)
	: m_value(value)
	, m_name(name)
{
}

std::string GlobalNode::name() const
{
	return m_name;
}

std::vector<DzResult> GlobalNode::build(const EntryPoint &entryPoint, Stack values) const
{
	return m_value->build(entryPoint, values);
}
