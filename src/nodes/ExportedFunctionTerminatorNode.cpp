#include <llvm/IR/IRBuilder.h>

#include "EntryPoint.h"
#include "IRBuilderEx.h"
#include "Utility.h"
#include "ValueHelper.h"

#include "nodes/ExportedFunctionTerminatorNode.h"

#include "values/ScalarValue.h"
#include "values/ReferenceValue.h"

std::vector<DzResult> ExportedFunctionTerminatorNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto context = entryPoint.context();

	auto function = entryPoint.function();
	auto previous = entryPoint.block();

	insertBlock(previous, function);

	auto block = llvm::BasicBlock::Create(*context, "entry", function);

	linkBlocks(previous, block);

	auto ep = entryPoint
		.withBlock(block);

	auto returnValue = ValueHelper::getScalar(ep, values);

	IRBuilderEx builder(ep);
	builder.createRet(*returnValue);

	return {{ ep, values }};
}
