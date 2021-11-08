#ifndef DZVALUE_H
#define DZVALUE_H

#include <llvm/IR/Value.h>

#include "Stack.h"
#include "Utility.h"
#include "EntryPoint.h"

struct DzResult
{
	EntryPoint entryPoint;
	Stack values;
};

class DzValue
{
	public:
		virtual bool compare(const DzValue *other, const EntryPoint &entryPoint) const;

		virtual std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const = 0;
};

#endif // DZVALUE_H
