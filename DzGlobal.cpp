#include <llvm/IR/Constants.h>

#include "DzGlobal.h"
#include "Type.h"

DzGlobal::DzGlobal(DzValue *value, const std::string &name)
	: m_value(value)
	, m_name(name)
{
}

int DzGlobal::compare(DzValue *other, const EntryPoint &entryPoint) const
{
	UNUSED(other);
	UNUSED(entryPoint);

	return -1;
}

std::string DzGlobal::name() const
{
	return m_name;
}

std::vector<DzResult> DzGlobal::build(const EntryPoint &entryPoint, Stack values) const
{
	return m_value->build(entryPoint, values);
}
