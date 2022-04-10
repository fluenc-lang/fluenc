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
void insertBlock(llvm::BasicBlock *source, llvm::Function *function);

template<typename TKey, typename TValue>
bool compareKey(const std::pair<TKey, TValue> &left, const std::pair<TKey, TValue> &right)
{
	return left.first < right.first;
};

#endif // UTILITY_H
