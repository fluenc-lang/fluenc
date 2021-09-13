#include <sstream>

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>

#include "DzTerminator.h"
#include "EntryPoint.h"

DzTerminator::DzTerminator(const std::string &name)
	: m_name(name)
{
}

std::vector<DzResult> DzTerminator::build(const EntryPoint &entryPoint, Stack values) const
{
	return {{ entryPoint, values }};
}
