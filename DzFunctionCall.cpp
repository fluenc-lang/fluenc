#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instructions.h>

#include "DzFunctionCall.h"
#include "DzFunction.h"
#include "EntryPoint.h"

DzFunctionCall::DzFunctionCall(DzValue *consumer, const std::string name, size_t numberOfArguments)
	: m_consumer(consumer)
	, m_name(name)
	, m_numberOfArguments(numberOfArguments)
{
}

std::vector<DzResult> DzFunctionCall::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();
	auto functions = entryPoint.functions();

	for (auto [i, end] = functions.equal_range(m_name); i != end; i++)
	{
		auto function = i->second;

		if (function->hasMatchingSignature(entryPoint, values, m_numberOfArguments))
		{
			std::vector<DzResult> result;

			auto functionResults = function->build(entryPoint, values);

			for (const auto &[iep, value] : functionResults)
			{
				auto block = llvm::BasicBlock::Create(*context);

				llvm::BranchInst::Create(block, iep.block());

				auto ep = entryPoint
					.withBlock(block);

				auto v = m_consumer->build(ep, value);

				for (const auto &f : v)
				{
					result.push_back(f);
				}
			}

			return result;
		}
	}

	throw new std::exception(); // TODO
}
