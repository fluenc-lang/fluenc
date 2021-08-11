#ifndef UTILITY_H
#define UTILITY_H

#include <llvm/ADT/StringRef.h>

#include <ostream>

template <typename T>
void UNUSED(T &&)
{
}

std::ostream& operator<<(std::ostream &stream, const llvm::StringRef &string);

#endif // UTILITY_H
