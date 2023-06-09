#ifndef UTILITY_H
#define UTILITY_H

#include <llvm/ADT/StringRef.h>
#include <llvm/IR/BasicBlock.h>

#include <ostream>

template <typename T>
void UNUSED(T &&)
{
}

std::ostream& operator<<(std::ostream &stream, const llvm::StringRef &string);

void linkBlocks(llvm::BasicBlock *source, llvm::BasicBlock *target);
void guardBranch(llvm::BasicBlock *block);

template<typename TKey, typename TValue>
bool compareKey(const std::pair<TKey, TValue> &left, const std::pair<TKey, TValue> &right)
{
	return left.first < right.first;
};

llvm::BasicBlock *createBlock(llvm::LLVMContext *context);

#endif // UTILITY_H
