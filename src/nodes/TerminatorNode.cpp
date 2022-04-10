#include <sstream>

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>

#include "TerminatorNode.h"
#include "EntryPoint.h"

TerminatorNode::TerminatorNode(const std::string &name, FunctionAttribute attribute)
	: m_name(name)
	, m_attribute(attribute)
{
}

std::vector<DzResult> TerminatorNode::build(const EntryPoint &entryPoint, Stack values) const
{
	return {{ entryPoint, values }};
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
