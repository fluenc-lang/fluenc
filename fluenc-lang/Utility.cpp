#include <sstream>

#include <llvm/IR/IRBuilder.h>

#include <fmt/core.h>

#include "Utility.h"

std::ostream& operator<<(std::ostream &stream, const llvm::StringRef &string)
{
	stream << string.str();

	return stream;
}

void linkBlocks(llvm::BasicBlock *source, llvm::BasicBlock *target)
{
	guardBranch(source);

	llvm::IRBuilder<> builder(source);
	builder.CreateBr(target);
}

void guardBranch(llvm::BasicBlock *block)
{
	for (auto &instruction : *block)
	{
		if (llvm::isa <llvm::BranchInst> (instruction))
		{
			throw std::exception();
		}
	}
}

llvm::BasicBlock *createBlock(llvm::LLVMContext *context)
{
	static int i;

	auto name = fmt::format("block_{}", i++);

	return llvm::BasicBlock::Create(*context, name);
}
