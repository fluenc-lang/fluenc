#include <llvm/IR/BasicBlock.h>

#include "DzFunctionCall.h"
#include "DzCallable.h"
#include "EntryPoint.h"

DzFunctionCall::DzFunctionCall(DzValue *consumer, const std::string name)
	: m_consumer(consumer)
	, m_name(name)
{
}

llvm::Value *DzFunctionCall::build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const
{
	auto &context = entryPoint.context();
	auto functions = entryPoint.functions();

	auto iterator = functions.find(m_name);

	if (iterator == functions.end())
	{
		return nullptr;
	}

	auto function = iterator->second;

	auto returnValue = function->build(entryPoint, values);

	values.push_back(returnValue);

	auto block = llvm::BasicBlock::Create(*context);

	auto ep = entryPoint
		.withBlock(block);

	return m_consumer->build(ep, values);
}
