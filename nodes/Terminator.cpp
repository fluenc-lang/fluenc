#include <sstream>

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>

#include "Terminator.h"
#include "EntryPoint.h"

Terminator::Terminator(const std::string &name, FunctionAttribute attribute)
	: m_name(name)
	, m_attribute(attribute)
{
}

std::vector<DzResult> Terminator::build(const EntryPoint &entryPoint, Stack values) const
{
	return {{ entryPoint, values }};
}

FunctionAttribute Terminator::attribute() const
{
	return m_attribute;
}

Terminator *Terminator::instance()
{
	static Terminator instance("pajs", FunctionAttribute::None);

	return &instance;
}
