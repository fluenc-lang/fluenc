#include <iostream>
#include <numeric>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include "DzFunctionCall.h"
#include "DzFunction.h"
#include "EntryPoint.h"
#include "Type.h"
#include "IndexIterator.h"
#include "AllIterator.h"
#include "IRBuilderEx.h"
#include "ZipIterator.h"

#include "values/DependentValue.h"
#include "values/TypedValue.h"
#include "values/ReferenceValue.h"
#include "values/TaintedValue.h"
#include "values/UserTypeValue.h"
#include "values/NamedValue.h"

DzFunctionCall::DzFunctionCall(const std::string name)
	: m_name(name)
{
}

int DzFunctionCall::order(const EntryPoint &entryPoint) const
{
	auto functions = entryPoint.functions();

	auto iterator = functions.find(m_name);

	if (iterator != functions.end())
	{
		if (iterator->second->attribute() == FunctionAttribute::Iterator)
		{
			return 1;
		}
	}

	return -1;
}

void transferValue(IRBuilderEx &builder
	, const BaseValue *value
	, const BaseValue *storage
	)
{
	if (auto reference = dynamic_cast<const ReferenceValue *>(value))
	{
		auto load = builder.createLoad(*reference);

		builder.createStore(load, *static_cast<const ReferenceValue *>(storage));
	}
	else if (auto userTypeValue = dynamic_cast<const UserTypeValue *>(value))
	{
		auto userTypeStorage = static_cast<const UserTypeValue *>(storage);

		auto valueFields = userTypeValue->fields();
		auto storageFields = userTypeStorage->fields();

		for (auto [valueField, storageField] : zip(valueFields, storageFields))
		{
			transferValue(builder
				, valueField->value()
				, storageField->value()
				);
		}
	}
}

std::vector<DzResult> DzFunctionCall::build(const EntryPoint &entryPoint, Stack values) const
{
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	block->insertInto(function);

	auto tailCallCandidate = entryPoint
		.byName(m_name);

	if (!tailCallCandidate)
	{
		return regularCall(entryPoint, values);
	}

	IRBuilderEx builder(entryPoint);

	auto targetValues = tailCallCandidate->values();
	auto inputValues = values;

	if (targetValues.size() != inputValues.size())
	{
		return regularCall(entryPoint, values);
	}

	auto result = true;

	std::transform(targetValues.begin(), targetValues.end(), inputValues.begin(), all_true(result), [=](auto storage, auto value)
	{
		auto storageType = storage->type();
		auto valueType = value->type();

		return valueType->is(storageType, entryPoint);
	});

	if (!result)
	{
		return regularCall(entryPoint, values);
	}

	auto tailCallTarget = findTailCallTarget(tailCallCandidate, inputValues);

	if (!tailCallTarget)
	{
		return regularCall(entryPoint, values);
	}

	for (auto [value, storage] : zip(inputValues, targetValues))
	{
		transferValue(builder, value, storage);
	}

	linkBlocks(block, tailCallTarget->block());

	return std::vector<DzResult>();
}

std::vector<DzResult> DzFunctionCall::regularCall(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();

	auto functions = entryPoint.functions();
	auto block = entryPoint.block();

	for (auto [i, end] = functions.equal_range(m_name); i != end; i++)
	{
		auto function = i->second;

		if (function->hasMatchingSignature(entryPoint, values))
		{
			auto functionBlock = llvm::BasicBlock::Create(*context, m_name);

			linkBlocks(block, functionBlock);

			auto functionEntryPoint = entryPoint
				.withBlock(functionBlock);

			if (function->attribute() == FunctionAttribute::Import)
			{
				return function->build(functionEntryPoint, values);
			}

			std::vector<DzResult> result;

			auto functionResults = function->build(functionEntryPoint, values);

			for (const auto &[lastEntryPoint, returnValue] : functionResults)
			{
				auto consumerBlock = llvm::BasicBlock::Create(*context);

				linkBlocks(lastEntryPoint.block(), consumerBlock);

				auto consumerEntryPoint = functionEntryPoint
					.withDepth(lastEntryPoint.depth())
					.withBlock(consumerBlock);

				result.push_back({ consumerEntryPoint, returnValue });
			}

			return result;
		}
	}

	throw new std::exception(); // TODO
}
