#include <iostream>
#include <numeric>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include "FunctionCall.h"
#include "EntryPoint.h"
#include "Type.h"
#include "IndexIterator.h"
#include "AllIterator.h"
#include "IRBuilderEx.h"
#include "ZipIterator.h"
#include "ValueHelper.h"
#include "FunctionNotFoundException.h"

#include "nodes/FunctionNode.h"

#include "values/DependentValue.h"
#include "values/ScalarValue.h"
#include "values/ReferenceValue.h"
#include "values/TaintedValue.h"
#include "values/UserTypeValue.h"
#include "values/NamedValue.h"
#include "values/ArrayValue.h"
#include "values/ExpandableValue.h"
#include "values/IteratorValue.h"
#include "values/FunctionValue.h"

#include "types/IteratorType.h"

FunctionCall::FunctionCall(antlr4::ParserRuleContext *context, const std::string &name)
	: m_context(context)
	, m_name(name)
{
}

int FunctionCall::order(const EntryPoint &entryPoint) const
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

std::vector<DzResult> FunctionCall::build(const EntryPoint &entryPoint, Stack values) const
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

const CallableNode *FunctionCall::findFunction(const EntryPoint &entryPoint, Stack values) const
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

	std::map<int8_t, CallableNode *> candidates;

	for (auto [i, end] = functions.equal_range(m_name); i != end; i++)
	{
		auto function = i->second;

		auto score = function->signatureCompatibility(entryPoint, values);

		if (score < 0)
		{
			continue;
		}

		auto candidate = candidates.find(score);

		if (candidate != candidates.end())
		{
			throw new std::exception();
		}

		candidates[score] = function;
	}

	if (candidates.size() > 0)
	{
		auto [_, function] = *candidates.begin();

		return function;
	}

	return nullptr;
}

std::vector<DzResult> FunctionCall::regularCall(const EntryPoint &entryPoint, Stack values) const
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

	throw new FunctionNotFoundException(m_context, m_name, values);
}
