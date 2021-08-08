#include <llvm/IR/IRBuilder.h>

#include "DzExportedFunctionTerminator.h"
#include "EntryPoint.h"

llvm::Value *DzExportedFunctionTerminator::build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const
{
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	block->setName("entry");
	block->insertInto(function);

	llvm::IRBuilder<> builder(block);

	auto returnValue = values.back();

	values.pop_back();

	builder.CreateRet(returnValue);

	for (auto i = function->begin(), j = std::next(i)
		 ; i != function->end() && j != function->end()
		 ; i++, j++
		 )
	{
		llvm::BranchInst::Create(&*j, &*i);
	}

	return function;
}
