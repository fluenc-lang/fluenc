#include <llvm/IR/IRBuilder.h>

#include <fmt/core.h>

#include "utility.hpp"

std::ostream& operator<<(std::ostream& stream, const llvm::StringRef& string)
{
	stream << string.str();

	return stream;
}

void link_blocks(llvm::BasicBlock* source, llvm::BasicBlock* target)
{
	guard_branch(source);

	llvm::IRBuilder<> builder(source);
	builder.CreateBr(target);
}

void guard_branch(llvm::BasicBlock* block)
{
#ifdef DEBUG
	for (auto& instruction : *block)
	{
		if (llvm::isa<llvm::BranchInst>(instruction))
		{
			throw std::exception();
		}
	}
#else
	UNUSED(block);
#endif
}

llvm::BasicBlock* create_block(llvm::LLVMContext* context)
{
	static int i;

	auto name = fmt::format("block_{}", i++);

	return llvm::BasicBlock::Create(*context, name);
}
