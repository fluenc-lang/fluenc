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

#include "values/DependentValue.h"
#include "values/TypedValue.h"

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

std::vector<DzResult> DzFunctionCall::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();

	auto functions = entryPoint.functions();
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	auto numberOfArguments = values.size();

	block->insertInto(function);

	auto tailCallCandidate = entryPoint
		.byName(m_name);

	if (tailCallCandidate)
	{
		llvm::IRBuilder<> builder(block);

		auto tailCallValues = tailCallCandidate->values();

		auto tailCallTarget = std::accumulate(index_iterator(0ul), index_iterator(numberOfArguments), tailCallCandidate, [&](const EntryPoint *target, size_t)
		{
			auto value = values.pop();
			auto storage = tailCallValues.pop();

			if (auto computedValue = dynamic_cast<const TypedValue *>(value))
			{
				auto load = builder.CreateLoad(*computedValue);

				builder.CreateStore(load, *static_cast<const TypedValue *>(storage));
			}
			else if (auto dependentValue = dynamic_cast<const DependentValue *>(value))
			{
				auto provider = dependentValue->provider();

				if (provider->depth() < target->depth())
				{
					return provider;
				}
			}

			return target;
		});

		linkBlocks(block, tailCallTarget->entry());

		return std::vector<DzResult>();
	}

	for (auto [i, end] = functions.equal_range(m_name); i != end; i++)
	{
		auto function = i->second;

		if (function->hasMatchingSignature(entryPoint, values))
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
	}

	throw new std::exception(); // TODO
}
