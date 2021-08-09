#include <map>
#include <algorithm>

#include "DzFunction.h"
#include "EntryPoint.h"

DzFunction::DzFunction(const std::string &name
	, std::vector<std::string> arguments
	, DzValue *block
	)
	: m_name(name)
	, m_arguments(arguments)
	, m_block(block)
{
}

std::string DzFunction::name() const
{
	return m_name;
}

FunctionAttribute DzFunction::attribute() const
{
	return FunctionAttribute::None;
}

Stack DzFunction::build(const EntryPoint &entryPoint, Stack values) const
{
	std::map<std::string, llvm::Value *> locals;

	for (const auto &argument : m_arguments)
	{
		locals[argument] = values.pop();
	}

	auto ep = entryPoint
		.withLocals(locals);

	return m_block->build(ep, values);
}
