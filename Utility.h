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

#endif // UTILITY_H
