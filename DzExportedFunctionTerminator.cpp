#include <llvm/IR/IRBuilder.h>

#include "DzExportedFunctionTerminator.h"
#include "EntryPoint.h"

std::vector<DzResult> DzExportedFunctionTerminator::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();

	auto function = entryPoint.function();
	auto previous = entryPoint.block();
	auto rva = entryPoint.returnValueAddress();

	auto block = llvm::BasicBlock::Create(*context, "entry", function);

	llvm::BranchInst::Create(block, previous);

	llvm::IRBuilder<> builder(block);

	auto returnValue = values.pop();

	builder.CreateStore(returnValue, rva);

	auto ep = entryPoint
		.withBlock(block);

	return {{ ep, values }};
}
