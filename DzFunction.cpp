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

llvm::Value *DzFunction::build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const
{
	std::map<std::string, llvm::Value *> locals;

	std::transform(begin(m_arguments), end(m_arguments), rbegin(values), std::inserter(locals, end(locals)), [](auto name, auto value)
	{
		return std::make_pair(name, value);
	});

	for (auto i = 0u; i < m_arguments.size(); i++)
	{
		values.pop_back();
	}

	auto ep = entryPoint
		.withLocals(locals);

	return m_block->build(ep, values);
}
