#include <llvm/IR/BasicBlock.h>

#include "DzFunctionCall.h"
#include "DzFunction.h"
#include "EntryPoint.h"

DzFunctionCall::DzFunctionCall(DzValue *consumer, const std::string name, size_t numberOfArguments)
	: m_consumer(consumer)
	, m_name(name)
	, m_numberOfArguments(numberOfArguments)
{
}

Stack DzFunctionCall::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();
	auto functions = entryPoint.functions();

	for (auto [i, end] = functions.equal_range(m_name); i != end; i++)
	{
		auto function = i->second;

		if (function->hasMatchingSignature(entryPoint, values, m_numberOfArguments))
		{
			auto returnValues = function->build(entryPoint, values);

			auto block = llvm::BasicBlock::Create(*context);

			auto ep = entryPoint
				.withBlock(block);

			return m_consumer->build(ep, returnValues);
		}
	}

	throw new std::exception(); // TODO
}
