#include "IteratorValue.h"

#include "types/IteratorType.h"

IteratorValue::IteratorValue(const DzValue *iterator, const Stack &values)
	: m_iterator(iterator)
	, m_values(values)
{
}

const Type *IteratorValue::type() const
{
	return IteratorType::instance(); // Idk
}

const BaseValue *IteratorValue::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}

std::vector<DzResult> IteratorValue::build(const EntryPoint &entryPoint) const
{
	auto &context = entryPoint.context();

	auto block = entryPoint.block();
	auto function = entryPoint.function();

	insertBlock(block, function);

	auto arrayBlock = llvm::BasicBlock::Create(*context);

	linkBlocks(block, arrayBlock);

	auto iteratorEntryPoint = entryPoint
		.withBlock(arrayBlock)
		.withName("__array")
		.markEntry()
		;

	std::vector<DzResult> results;

	for (auto &result : m_iterator->build(iteratorEntryPoint, m_values))
	{
		results.push_back(result);
	}

	return results;
}
