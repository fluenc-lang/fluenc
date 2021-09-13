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

	std::transform(begin(m_arguments), end(m_arguments), values.rbegin(), all_true(result), [=](DzArgument *argument, auto value)
	{
		if (!value)
		{
			return false;
		}

		auto argumentType = argument->type(entryPoint);
		auto valueType = value.type();

		return argumentType->tag() == valueType->tag();
	});

	return result;
}

std::vector<DzResult> DzFunction::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	std::map<std::string, TypedValue> locals;

	for (const auto &argument : m_arguments)
	{
		auto name = argument->name();

		auto addressOfArgument = values.pop();

		auto argumentType = addressOfArgument.type();

		if (auto userType = dynamic_cast<UserType *>(argumentType))
		{
			auto prototype = userType->prototype();

			auto i = 0;

			for (auto &field : prototype->fields())
			{
				auto intType = llvm::Type::getInt32Ty(*context);

				llvm::Value *indexes[] =
				{
					llvm::ConstantInt::get(intType, 0),
					llvm::ConstantInt::get(intType, i++)
				};

				std::stringstream ss;
				ss << name;
				ss << ".";
				ss << field;

				auto align = dataLayout.getABITypeAlign(intType);

				auto gep = llvm::GetElementPtrInst::CreateInBounds(addressOfArgument, indexes, field, block);

				auto load = new llvm::LoadInst(intType, gep, name, false, align, block);

				auto localName = ss.str();

				locals[localName] = TypedValue(Int32Type::instance(), load);
			}
		}
		else
		{
			auto storageType = argumentType->storageType(*context);
			auto align = dataLayout.getABITypeAlign(storageType);

			auto load = new llvm::LoadInst(storageType, addressOfArgument, name, false, align, block);

			locals[name] = TypedValue(argumentType, load);
		}
	}

	auto ep = entryPoint
		.withName(m_name)
		.withLocals(locals);

	return m_block->build(ep, values);
}
