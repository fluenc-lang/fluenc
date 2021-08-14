#include <map>
#include <algorithm>
#include <sstream>

#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

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

	std::transform(begin(m_arguments), end(m_arguments), values.rbegin(), all_true(result), [=](auto argument, auto value)
	{
		if (!value)
		{
			return false;
		}

		return argument->type(entryPoint) == value->getType();
	});

	return result;
}

std::vector<DzResult> DzFunction::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	std::map<std::string, llvm::Value *> locals;

	for (const auto &argument : m_arguments)
	{
		auto name = argument->name();
		auto type = argument->type(entryPoint);

		auto addressOfArgument = values.pop();

		auto align = dataLayout.getABITypeAlign(type);

		auto load = new llvm::LoadInst(type, addressOfArgument, name, false, align, block);

		locals[name] = load;
	}

	auto ep = entryPoint
		.withName(m_name)
		.withLocals(locals);

	return m_block->build(ep, values);
}
