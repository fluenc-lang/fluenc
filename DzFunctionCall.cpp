#include <llvm/IR/BasicBlock.h>

#include "DzFunctionCall.h"
#include "DzCallable.h"
#include "EntryPoint.h"

DzFunctionCall::DzFunctionCall(DzValue *consumer, const std::string name)
	: m_consumer(consumer)
	, m_name(name)
{
}

Stack DzFunctionCall::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();
	auto functions = entryPoint.functions();

	auto iterator = functions.find(m_name);

	if (iterator == functions.end())
	{
		return values;
	}

	auto function = iterator->second;

	auto returnValues = function->build(entryPoint, values);

	auto block = llvm::BasicBlock::Create(*context);

	auto ep = entryPoint
		.withBlock(block);

	return m_consumer->build(ep, returnValues);
}
