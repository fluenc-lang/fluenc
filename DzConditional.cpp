#include <sstream>

#include <llvm/IR/IRBuilder.h>

#include "DzConditional.h"
#include "EntryPoint.h"

#include "values/TypedValue.h"
#include "values/ReferenceValue.h"

DzConditional::DzConditional(DzValue *ifFalse, DzValue *ifTrue)
	: m_ifTrue(ifTrue)
	, m_ifFalse(ifFalse)
{
}

std::vector<DzResult> DzConditional::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto function = entryPoint.function();
	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	block->setName("condition");
	block->insertInto(function);

	auto ifTrue = llvm::BasicBlock::Create(*context);
	auto ifFalse = llvm::BasicBlock::Create(*context);

	llvm::IRBuilder<> builder(block);

	builder.CreateCondBr(*values.require<TypedValue>(), ifTrue, ifFalse);

	auto epIfFalse = entryPoint
		.withName("ifFalse")
		.withBlock(ifFalse);

	auto epIfTrue = entryPoint
		.withName("ifTrue")
		.withBlock(ifTrue);

	auto resultsIfTrue = m_ifTrue->build(epIfTrue, values);
	auto resultsIfFalse = m_ifFalse->build(epIfFalse, values);

	std::vector<DzResult> result;

	result.insert(end(result), begin(resultsIfTrue), end(resultsIfTrue));
	result.insert(end(result), begin(resultsIfFalse), end(resultsIfFalse));

	if (resultsIfTrue.size() != 1 || resultsIfFalse.size() != 1)
	{
		return result;
	}

	auto [resultEpIfTrue, valuesIfTrue] = *resultsIfTrue.begin();
	auto [resultEpIfFalse, valuesIfFalse] = *resultsIfFalse.begin();

	auto valueIfTrue = valuesIfTrue.request<TypedValue>();
	auto valueIfFalse = valuesIfFalse.request<TypedValue>();

	if (!valueIfTrue || !valueIfFalse)
	{
		return result;
	}

	if (valueIfTrue->type() != valueIfFalse->type())
	{
		return result;
	}

	auto resultBlockIfTrue = resultEpIfTrue.block();
	auto resultBlockIfFalse = resultEpIfFalse.block();

	resultBlockIfTrue->insertInto(function);
	resultBlockIfFalse->insertInto(function);

	auto type = valueIfTrue->type();
	auto storageType = type->storageType(*context);

	auto align = dataLayout.getABITypeAlign(storageType);

	auto alloc = entryPoint.alloc(storageType);

	auto mergeBlock = llvm::BasicBlock::Create(*context, "merge", function);

	auto storeIfTrue = new llvm::StoreInst(*valueIfTrue, alloc, false, align, resultBlockIfTrue);
	auto storeIfFalse = new llvm::StoreInst(*valueIfFalse, alloc, false, align, resultBlockIfFalse);

	UNUSED(storeIfTrue);
	UNUSED(storeIfFalse);

	linkBlocks(resultBlockIfTrue, mergeBlock);
	linkBlocks(resultBlockIfFalse, mergeBlock);

	auto mergeLoad = new llvm::LoadInst(storageType, alloc, "mergeLoad", false, align, mergeBlock);

	auto mergeEntryPoint = entryPoint
		.withBlock(mergeBlock);

	auto mergeResult = new TypedValue { type, mergeLoad };

	values.push(mergeResult);

	return {{ mergeEntryPoint, values }};
}
