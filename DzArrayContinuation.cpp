#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include "DzArrayContinuation.h"

#include "types/Int64Type.h"

DzArrayContinuation::DzArrayContinuation(llvm::Value *index)
	: m_index(index)
{
}

std::vector<DzResult> DzArrayContinuation::build(const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	auto indexType = Int64Type::instance();
	auto storageType = indexType->storageType(*context);

	auto align = dataLayout.getABITypeAlign(storageType);

	llvm::IRBuilder<> builder(block);

	auto load = new llvm::LoadInst(storageType, m_index, "index", false, align, block);
	auto indexConstant = llvm::ConstantInt::get(storageType, 1);
	auto add = builder.CreateAdd(load, indexConstant);
	auto store = new llvm::StoreInst(add, m_index, false, align, block);

	UNUSED(store);

	return std::vector<DzResult>();
}
