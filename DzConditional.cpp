#include <sstream>

#include <llvm/IR/IRBuilder.h>

#include "DzConditional.h"
#include "EntryPoint.h"

DzConditional::DzConditional(DzValue *ifFalse, DzValue *ifTrue, DzValue *condition, DzValue *consumer)
	: m_ifTrue(ifTrue)
	, m_ifFalse(ifFalse)
	, m_condition(condition)
	, m_consumer(consumer)
{
}

std::vector<DzResult> DzConditional::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	block->setName("condition");
	block->insertInto(function);

	auto conditionValues = m_condition->build(entryPoint, values);

	auto ifTrue = llvm::BasicBlock::Create(*context);
	auto ifFalse = llvm::BasicBlock::Create(*context);

	llvm::IRBuilder<> builder(block);

	for (auto &[_, value] : conditionValues)
	{
		builder.CreateCondBr(value.pop(), ifTrue, ifFalse);
	}

	auto epIfFalse = entryPoint
		.withName("ifFalse")
		.withBlock(ifFalse);

	auto epIfTrue = entryPoint
		.withName("ifTrue")
		.withBlock(ifTrue);

	auto valuesIfTrue = m_ifTrue->build(epIfTrue, values);
	auto valuesIfFalse = m_ifFalse->build(epIfFalse, values);

	std::vector<DzResult> result;

	result.insert(end(result), begin(valuesIfTrue), end(valuesIfTrue));
	result.insert(end(result), begin(valuesIfFalse), end(valuesIfFalse));

	return result;
}
