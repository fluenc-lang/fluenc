#ifndef INTEROPHELPER_H
#define INTEROPHELPER_H

#include <llvm/IR/Value.h>

class BaseValue;
class Type;
class EntryPoint;
class UserTypeValue;

class InteropHelper
{
	public:
		static const BaseValue *createReadProxy(llvm::Value *value, const Type *type, const EntryPoint &entryPoint);
		static llvm::Value *createWriteProxy(const UserTypeValue *userTypeValue, const EntryPoint &entryPoint);
};

#endif // INTEROPHELPER_H
