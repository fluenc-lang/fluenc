#include <llvm/IR/IRBuilder.h>

#include "ImportedFunctionNode.h"
#include "DzTypeName.h"
#include "DzArgument.h"
#include "AllIterator.h"
#include "Type.h"
#include "IndexIterator.h"
#include "InteropHelper.h"
#include "IRBuilderEx.h"

#include "types/VoidType.h"
#include "types/Prototype.h"

#include "values/ScalarValue.h"
#include "values/UserTypeValue.h"
#include "values/ReferenceValue.h"

#include "exceptions/InvalidArgumentTypeException.h"

ImportedFunctionNode::ImportedFunctionNode(antlr4::ParserRuleContext *context
	, const std::string &name
	, const std::vector<DzBaseArgument *> &arguments
	, ITypeName *returnType
	)
	: m_context(context)
	, m_name(name)
	, m_arguments(arguments)
	, m_returnType(returnType)
{
}

std::string ImportedFunctionNode::name() const
{
	return m_name;
}

std::vector<DzBaseArgument *> ImportedFunctionNode::arguments() const
{
	return m_arguments;
}

FunctionAttribute ImportedFunctionNode::attribute() const
{
	return FunctionAttribute::Import;
}

bool ImportedFunctionNode::hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const
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

int8_t ImportedFunctionNode::signatureCompatibility(const EntryPoint &entryPoint, const Stack &values) const
{
	if (hasMatchingSignature(entryPoint, values))
	{
		return 0;
	}

	return -1;
}

std::vector<DzResult> ImportedFunctionNode::build(const EntryPoint &entryPoint, Stack values) const
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
			throw new InvalidArgumentTypeException(m_context);
		}
	}

	llvm::FunctionType *functionType = llvm::FunctionType::get(returnType->storageType(*context), argumentTypes, false);

	auto function = module->getOrInsertFunction(m_name, functionType);

	auto call = builder.createCall(function, argumentValues);

	if (returnType != VoidType::instance())
	{
		auto returnValue = InteropHelper::createReadProxy(call, returnType, entryPoint, m_context);

		values.push(returnValue);
	}

	return {{ entryPoint, values }};
}
