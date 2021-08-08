#ifndef DZVALUE_H
#define DZVALUE_H

#include <deque>

#include <llvm/IR/Value.h>

class EntryPoint;

class DzValue
{
	public:
		virtual llvm::Value *build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const = 0;
};

#endif // DZVALUE_H
