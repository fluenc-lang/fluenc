#ifndef DZVALUENG_H
#define DZVALUENG_H

#include <llvm/IR/Value.h>

class EntryPointInfo;

class DzValueNg
{
	public:
		virtual llvm::Value *build(const EntryPointInfo &entryPoint) const = 0;
};
#endif // DZVALUENG_H
