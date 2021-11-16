#include <llvm/IR/IRBuilder.h>

#include "DzImportedFunction.h"
#include "DzTypeName.h"
#include "DzArgument.h"
#include "AllIterator.h"
#include "Type.h"

#include "types/VoidType.h"
#include "types/UserType.h"
#include "types/Prototype.h"

#include "values/TypedValue.h"

DzImportedFunction::DzImportedFunction(const std::string &name
	, const std::vector<DzBaseArgument *> &arguments
	, DzTypeName *returnType
	)
	: m_name(name)
	, m_arguments(arguments)
	, m_returnType(returnType)
{
}

std::string DzImportedFunction::name() const
{
	return m_name;
}

FunctionAttribute DzImportedFunction::attribute() const
{
	return FunctionAttribute::Import;
}

bool DzImportedFunction::hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const
{
	if (m_arguments.size() != values.size())
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

		auto argumentType = argument->type(entryPoint);
		auto valueType = value->type();

		return valueType->is(argumentType, entryPoint);
	});

	return result;
}

std::vector<DzResult> DzImportedFunction::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	auto returnType = m_returnType->resolve(entryPoint);

	std::vector<llvm::Type *> argumentTypes;
	std::vector<llvm::Value *> argumentValues;

	for (const auto &argument : m_arguments)
	{
		if (auto standardArgument  = dynamic_cast<DzArgument *>(argument))
		{
			auto name = standardArgument->name();
			auto type = standardArgument->type(entryPoint);

			auto storageType = type->storageType(*context);

			argumentTypes.push_back(storageType);

			auto addressOfArgument = values.require<TypedValue>();

			auto align = dataLayout.getABITypeAlign(storageType);

			auto load = new llvm::LoadInst(storageType, *addressOfArgument, name, false, align, block);

			if (dynamic_cast<Prototype *>(type))
			{
				auto cast = new llvm::BitCastInst(load, llvm::Type::getInt8PtrTy(*context), "cast", block);

				argumentValues.push_back(cast);
			}
			else
			{
				argumentValues.push_back(load);
			}
		}
		else
		{
			throw new std::exception();
		}
	}

	llvm::FunctionType *functionType = llvm::FunctionType::get(returnType->storageType(*context), argumentTypes, false);

	auto function = module->getOrInsertFunction(m_name, functionType);

	llvm::IRBuilder<> builder(block);

	auto call = builder.CreateCall(function, argumentValues);

	if (returnType != VoidType::instance())
	{
		values.push(new TypedValue { returnType, call });
	}

	return {{ entryPoint, values }};
}
