#include <llvm/IR/IRBuilder.h>

#include "nodes/ExportedFunctionTerminatorNode.h"
#include "EntryPoint.h"
#include "IRBuilderEx.h"

#include "values/ScalarValue.h"

std::vector<DzResult> ExportedFunctionTerminatorNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();

	auto function = entryPoint.function();
	auto previous = entryPoint.block();

	insertBlock(previous, function);

	auto block = llvm::BasicBlock::Create(*context, "entry", function);

	linkBlocks(previous, block);

	auto ep = entryPoint
		.withBlock(block);

	auto returnValue = values.require<ScalarValue>();

	IRBuilderEx builder(ep);
	builder.createRet(*returnValue);

	return {{ ep, values }};
}
