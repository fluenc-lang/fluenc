#include <iostream>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include "DzFunctionCall.h"
#include "DzFunction.h"
#include "EntryPoint.h"
#include "Type.h"

DzFunctionCall::DzFunctionCall(const std::string name, size_t numberOfArguments)
	: m_name(name)
	, m_numberOfArguments(numberOfArguments)
{
}

int DzFunctionCall::compare(DzValue *other, const EntryPoint &entryPoint) const
{
	UNUSED(other);
	UNUSED(entryPoint);

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

	auto tailCallTarget = entryPoint
		.byName(m_name);

	if (tailCallTarget)
	{
		llvm::IRBuilder<> builder(block);

		auto tailCallValues = tailCallTarget->values();

		for (auto i = 0u; i < numberOfArguments; i++)
		{
			builder.CreateStore(values.pop(), tailCallValues.pop());
		}

		linkBlocks(block, tailCallTarget->entry());

		return std::vector<DzResult>();
	}

	for (auto [i, end] = functions.equal_range(m_name); i != end; i++)
	{
		auto function = i->second;

		if (function->hasMatchingSignature(entryPoint, values))
		{
			std::vector<TypedValue> argumentValues;

			for (auto i = 0u; i < numberOfArguments; i++)
			{
				auto value = values.pop();

				argumentValues.push_back(value);
			}

			for (auto i = rbegin(argumentValues); i != rend(argumentValues); i++)
			{
				llvm::IRBuilder<> builder(block);

				auto value = *i;

				auto argumentType = value.type();
				auto storageType = argumentType->storageType(*context);

				auto alloc = entryPoint.alloc(storageType);

				builder.CreateStore(value, alloc);

				values.push({ argumentType, alloc });
			}

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
					.withBlock(consumerBlock);

				result.push_back({ consumerEntryPoint, returnValue });
			}

			return result;
		}
	}

	throw new std::exception(); // TODO
}
