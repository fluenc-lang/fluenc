#include <map>
#include <algorithm>
#include <sstream>

#include <llvm/IR/Type.h>

#include "DzFunction.h"
#include "DzArgument.h"
#include "DzTypeName.h"
#include "EntryPoint.h"
#include "AllIterator.h"

DzFunction::DzFunction(const std::string &name
	, std::vector<DzArgument *> arguments
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

bool DzFunction::hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values, size_t numberOfArguments) const
{
	// Sanity check.
	// TODO: Introduce slicing of stack, to only expose a sane amount of values.
	// Then this parameter (and check) can be removed.

	if (m_arguments.size() != numberOfArguments)
	{
		return false;
	}

	auto result = true;

	std::transform(begin(m_arguments), end(m_arguments), values.begin(), all_true(result), [=](auto argument, auto value)
	{
		if (!value)
		{
			return false;
		}

		return argument->type()->resolve(entryPoint) == value->getType();
	});

	return result;
}

Stack DzFunction::build(const EntryPoint &entryPoint, Stack values) const
{
	std::map<std::string, llvm::Value *> locals;

	for (const auto &argument : m_arguments)
	{
		auto name = argument->name();

		locals[name] = values.pop();
	}

	auto ep = entryPoint
		.withLocals(locals);

	return m_block->build(ep, values);
}
