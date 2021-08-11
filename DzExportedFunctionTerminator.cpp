#include <llvm/IR/IRBuilder.h>

#include "DzExportedFunctionTerminator.h"
#include "EntryPoint.h"

std::vector<DzResult> DzExportedFunctionTerminator::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();
	auto function = entryPoint.function();
	auto b = entryPoint.block();

	auto block = llvm::BasicBlock::Create(*context, "entry", function);

	llvm::BranchInst::Create(block, b);

	llvm::IRBuilder<> builder(block);

	auto returnValue = values.pop();

	builder.CreateRet(returnValue);

	return std::vector<DzResult>();
}
