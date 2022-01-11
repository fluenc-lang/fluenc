#include <llvm/IR/IRBuilder.h>

#include "DzExportedFunctionTerminator.h"
#include "EntryPoint.h"
#include "IRBuilderEx.h"

#include "values/TypedValue.h"

std::vector<DzResult> DzExportedFunctionTerminator::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();

	auto function = entryPoint.function();
	auto previous = entryPoint.block();

	auto block = llvm::BasicBlock::Create(*context, "entry", function);

	linkBlocks(previous, block);

	auto ep = entryPoint
		.withBlock(block);

	auto returnValue = values.require<TypedValue>();

	IRBuilderEx builder(ep);
	builder.createRet(*returnValue);

	return {{ ep, values }};
}
