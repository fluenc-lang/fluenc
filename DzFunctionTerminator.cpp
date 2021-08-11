#include <sstream>

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>

#include "DzFunctionTerminator.h"
#include "EntryPoint.h"

DzFunctionTerminator::DzFunctionTerminator(const std::string &name)
	: m_name(name)
{
}

std::vector<DzResult> DzFunctionTerminator::build(const EntryPoint &entryPoint, Stack values) const
{
	return {{ entryPoint, values }};
}
