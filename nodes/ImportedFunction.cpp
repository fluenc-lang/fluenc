#include <llvm/IR/IRBuilder.h>

#include "ImportedFunction.h"
#include "DzTypeName.h"
#include "DzArgument.h"
#include "AllIterator.h"
#include "Type.h"
#include "IndexIterator.h"
#include "InteropHelper.h"
#include "IRBuilderEx.h"

#include "types/VoidType.h"
#include "types/Prototype.h"

#include "values/TypedValue.h"
#include "values/UserTypeValue.h"
#include "values/ReferenceValue.h"

ImportedFunction::ImportedFunction(const std::string &name
	, const std::vector<DzBaseArgument *> &arguments
	, ITypeName *returnType
	)
	: m_name(name)
	, m_arguments(arguments)
	, m_returnType(returnType)
{
}

std::string ImportedFunction::name() const
{
	return m_name;
}

std::vector<DzBaseArgument *> ImportedFunction::arguments() const
{
	return m_arguments;
}

FunctionAttribute ImportedFunction::attribute() const
{
	return FunctionAttribute::Import;
}

bool ImportedFunction::hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const
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

int8_t ImportedFunction::signatureCompatibility(const EntryPoint &entryPoint, const Stack &values) const
{
	if (hasMatchingSignature(entryPoint, values))
	{
		return 0;
	}

	return -1;
}

std::vector<DzResult> ImportedFunction::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto returnType = m_returnType->resolve(entryPoint);

	std::vector<llvm::Type *> argumentTypes;
	std::vector<llvm::Value *> argumentValues;

	IRBuilderEx builder(entryPoint);

	for (const auto &argument : m_arguments)
	{
		if (auto standardArgument  = dynamic_cast<DzArgument *>(argument))
		{
			auto name = standardArgument->name();
			auto type = standardArgument->type(entryPoint);

			auto storageType = type->storageType(*context);

			argumentTypes.push_back(storageType);

			auto value = values.pop();

			if (auto addressOfArgument = dynamic_cast<const ReferenceValue *>(value))
			{
				auto load = builder.createLoad(*addressOfArgument, name);

				argumentValues.push_back(load);
			}
			else if (auto userTypeValue = dynamic_cast<const UserTypeValue *>(value))
			{
				auto cast = InteropHelper::createWriteProxy(userTypeValue, entryPoint);

				argumentValues.push_back(cast);
			}
		}
		else
		{
			throw new std::exception();
		}
	}

	llvm::FunctionType *functionType = llvm::FunctionType::get(returnType->storageType(*context), argumentTypes, false);

	auto function = module->getOrInsertFunction(m_name, functionType);

	auto call = builder.createCall(function, argumentValues);

	if (returnType != VoidType::instance())
	{
		auto returnValue = InteropHelper::createReadProxy(call, returnType, entryPoint);

		values.push(returnValue);
	}

	return {{ entryPoint, values }};
}
