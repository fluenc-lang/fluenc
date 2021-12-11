#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>

#include "DzArrayInit.h"

#include "types/Int64Type.h"
#include "values/TypedValue.h"

DzArrayInit::DzArrayInit(DzValue *consumer)
	: m_consumer(consumer)
{
}

int DzArrayInit::order(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return 1;
}

std::vector<DzResult> DzArrayInit::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto block = entryPoint.block();
	auto function = entryPoint.function();

	block->insertInto(function);

	auto dataLayout = module->getDataLayout();

	auto indexType = Int64Type::instance();
	auto storageType = indexType->storageType(*context);

	auto align = dataLayout.getABITypeAlign(storageType);

	auto zero = llvm::ConstantInt::get(storageType, 0);

	auto index = entryPoint.alloc(storageType);

	auto store = new llvm::StoreInst(zero, index, false, align, block);

	UNUSED(store);

	values.push(new TypedValue { indexType, index });

	auto arrayBlock = llvm::BasicBlock::Create(*context);

	linkBlocks(block, arrayBlock);

	auto ep = entryPoint
		.withBlock(arrayBlock)
		.markEntry()
		;

	return m_consumer->build(ep, values);
}
