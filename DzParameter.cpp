#include <sstream>

#include "DzParameter.h"
#include "EntryPoint.h"

DzParameter::DzParameter(int id)
	: m_id(id)
{
}

llvm::Value *DzParameter::build(const EntryPoint &entryPoint) const
{
	// TODO! Implementation

	std::ostringstream stream;
	stream << "closure_";
	stream << m_id;

	auto module = entryPoint.module();
	auto function = module->getFunction(stream.str());

	return function->getArg(0);
}
