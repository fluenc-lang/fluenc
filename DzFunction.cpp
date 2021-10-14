#include <map>
#include <algorithm>
#include <sstream>
#include <iostream>

#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

#include "DzFunction.h"
#include "DzArgument.h"
#include "DzTypeName.h"
#include "EntryPoint.h"
#include "AllIterator.h"
#include "Type.h"

#include "types/UserType.h"

DzFunction::DzFunction(FunctionAttribute attribute
	, const std::string &name
	, const std::vector<DzArgument *> &arguments
	, DzValue *block
	)
	: m_attribute(attribute)
	, m_name(name)
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
	return m_attribute;
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

	std::transform(begin(m_arguments), end(m_arguments), values.rbegin(), all_true(result), [=](DzArgument *argument, auto value)
	{
		if (!value)
		{
			return false;
		}

		auto argumentType = argument->type(entryPoint);
		auto valueType = value.type();

		return valueType->is(argumentType, entryPoint);
	});

	return result;
}

std::vector<DzResult> DzFunction::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto pep = entryPoint
		.withValues(values);

	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	auto locals = entryPoint.locals();

	for (const auto &argument : m_arguments)
	{
		auto name = argument->name();

		auto addressOfArgument = values.pop();

		auto argumentType = addressOfArgument.type();
		auto storageType = argumentType->storageType(*context);

		auto align = dataLayout.getABITypeAlign(storageType);

		if (auto userType = dynamic_cast<UserType *>(argumentType))
		{
			auto load = new llvm::LoadInst(storageType, addressOfArgument, name, false, align, block);

			auto i = 0;

			for (auto &field : userType->fields())
			{
				auto intType = llvm::Type::getInt32Ty(*context);

				auto fieldType = field.type();

				llvm::Value *indexes[] =
				{
					llvm::ConstantInt::get(intType, 0),
					llvm::ConstantInt::get(intType, i++)
				};

				std::stringstream ss;
				ss << name;
				ss << ".";
				ss << field.name();

				auto gep = llvm::GetElementPtrInst::CreateInBounds(load, indexes, field.name(), block);

				auto localName = ss.str();

				locals[localName] = TypedValue(fieldType, gep);
			}

			locals[name] = addressOfArgument;
		}
		else
		{
			locals[name] = TypedValue(argumentType, addressOfArgument);
		}
	}

	auto ep = pep
		.withName(m_name)
		.withEntry(block)
		.withLocals(locals);

	return m_block->build(ep, values);
}
