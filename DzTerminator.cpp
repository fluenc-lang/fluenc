#include <sstream>

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>

#include "DzTerminator.h"
#include "EntryPoint.h"

DzTerminator::DzTerminator(const std::string &name)
	: m_name(name)
{
}

int DzTerminator::compare(DzValue *other, const EntryPoint &entryPoint) const
{
	UNUSED(other);
	UNUSED(entryPoint);

	return -1;
}

std::vector<DzResult> DzTerminator::build(const EntryPoint &entryPoint, Stack values) const
{
	return {{ entryPoint, values }};
}

DzTerminator *DzTerminator::instance()
{
	static DzTerminator instance("pajs");

	return &instance;
}
