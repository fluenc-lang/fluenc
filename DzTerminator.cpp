#include <sstream>

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>

#include "DzTerminator.h"
#include "EntryPoint.h"

DzTerminator::DzTerminator(const std::string &name, FunctionAttribute attribute)
	: m_name(name)
	, m_attribute(attribute)
{
}

std::vector<DzResult> DzTerminator::build(const EntryPoint &entryPoint, Stack values) const
{
	return {{ entryPoint, values }};
}

FunctionAttribute DzTerminator::attribute() const
{
	return m_attribute;
}

DzTerminator *DzTerminator::instance()
{
	static DzTerminator instance("pajs", FunctionAttribute::None);

	return &instance;
}
