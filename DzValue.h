#ifndef DZVALUE_H
#define DZVALUE_H

#include <llvm/IR/Value.h>

#include "Stack.h"

class EntryPoint;

class DzValue
{
	public:
		virtual Stack build(const EntryPoint &entryPoint, Stack values) const = 0;
};

#endif // DZVALUE_H
