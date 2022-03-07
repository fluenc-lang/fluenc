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
#include "ValueHelper.h"

#include "values/DependentValue.h"
#include "values/TypedValue.h"
#include "values/ReferenceValue.h"
#include "values/TaintedValue.h"
#include "values/UserTypeValue.h"
#include "values/NamedValue.h"
#include "values/ArrayValue.h"
#include "values/ExpandableValue.h"
#include "values/LazyValue.h"
#include "values/FunctionValue.h"

#include "types/IteratorType.h"

DzFunctionCall::DzFunctionCall(const std::string &name)
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

std::vector<DzResult> DzFunctionCall::build(const EntryPoint &entryPoint, Stack values) const
{
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	insertBlock(block, function);

	auto tailCallCandidate = entryPoint
		.byName(m_name);

	if (!tailCallCandidate)
	{
		return regularCall(entryPoint, values);
	}

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

		return valueType->equals(storageType, entryPoint);
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

	auto zipped = zip(inputValues, targetValues);

	auto resultEntryPoint = std::accumulate(zipped.begin(), zipped.end(), entryPoint, [&](auto accumulatedEntryPoint, auto result)
	{
		auto [value, storage] = result;

		return ValueHelper::transferValue(accumulatedEntryPoint, value, storage);
	});

	linkBlocks(resultEntryPoint.block(), tailCallTarget->block());

	return std::vector<DzResult>();
}

const DzCallable *DzFunctionCall::findFunction(const EntryPoint &entryPoint, Stack values) const
{
	auto functions = entryPoint.functions();
	auto locals = entryPoint.locals();

	auto local = locals.find(m_name);

	if (local != locals.end())
	{
		auto value = dynamic_cast<const FunctionValue *>(local->second);

		if (!value)
		{
			throw new std::exception();
		}

		return value->function();
	}

	for (auto [i, end] = functions.equal_range(m_name); i != end; i++)
	{
		auto function = i->second;

		if (function->hasMatchingSignature(entryPoint, values))
		{
			return function;
		}
	}

	return nullptr;
}

std::vector<DzResult> DzFunctionCall::regularCall(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();

	auto functions = entryPoint.functions();
	auto locals = entryPoint.locals();

	auto block = entryPoint.block();

	auto function = findFunction(entryPoint, values);

	if (function)
	{
		auto functionBlock = llvm::BasicBlock::Create(*context);

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

	throw new std::exception(); // TODO
}
