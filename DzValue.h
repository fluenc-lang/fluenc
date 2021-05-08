#ifndef DZVALUE_H
#define DZVALUE_H

#include <string>

#include <llvm/IR/BasicBlock.h>

class EntryPoint;

class DzValue
{
	public:
		virtual llvm::Value *build(const EntryPoint &entryPoint) const = 0;
};

#endif // DZVALUE_H
