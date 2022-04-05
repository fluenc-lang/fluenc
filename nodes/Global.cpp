#include <llvm/IR/Constants.h>

#include "Global.h"
#include "Type.h"

Global::Global(Node *value, const std::string &name)
	: m_value(value)
	, m_name(name)
{
}

std::string Global::name() const
{
	return m_name;
}

std::vector<DzResult> Global::build(const EntryPoint &entryPoint, Stack values) const
{
	return m_value->build(entryPoint, values);
}
