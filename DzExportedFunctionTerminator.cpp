#include <llvm/IR/IRBuilder.h>

#include "DzExportedFunctionTerminator.h"
#include "EntryPoint.h"

Stack DzExportedFunctionTerminator::build(const EntryPoint &entryPoint, Stack values) const
{
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	block->setName("entry");
	block->insertInto(function);

	llvm::IRBuilder<> builder(block);

	auto returnValue = values.pop();

	builder.CreateRet(returnValue);

	for (auto i = function->begin(), j = std::next(i)
		 ; i != function->end() && j != function->end()
		 ; i++, j++
		 )
	{
		llvm::BranchInst::Create(&*j, &*i);
	}

	return values;
}
