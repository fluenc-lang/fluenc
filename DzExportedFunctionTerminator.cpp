#include <llvm/IR/IRBuilder.h>

#include "DzExportedFunctionTerminator.h"
#include "EntryPoint.h"

#include "values/TypedValue.h"

std::vector<DzResult> DzExportedFunctionTerminator::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();

	auto function = entryPoint.function();
	auto previous = entryPoint.block();

	auto block = llvm::BasicBlock::Create(*context, "entry", function);

	linkBlocks(previous, block);

	auto returnValue = values.require<TypedValue>();

	llvm::IRBuilder<> builder(block);
	builder.CreateRet(*returnValue);

	return {{ entryPoint, values }};
}
