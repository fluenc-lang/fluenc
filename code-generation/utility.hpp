#pragma once

#include <llvm/ADT/StringRef.h>
#include <llvm/IR/BasicBlock.h>

#include <ostream>

template <typename T>
void UNUSED(T&&)
{
}

std::ostream& operator<<(std::ostream& stream, const llvm::StringRef& string);

void link_blocks(llvm::BasicBlock* source, llvm::BasicBlock* target);
void guard_branch(llvm::BasicBlock* block);

template <typename TKey, typename TValue>
bool compare_key(const std::pair<TKey, TValue>& left, const std::pair<TKey, TValue>& right)
{
	return left.first < right.first;
};

llvm::BasicBlock* create_block(llvm::LLVMContext* context);
