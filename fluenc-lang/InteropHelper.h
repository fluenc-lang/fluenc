#ifndef INTEROPHELPER_H
#define INTEROPHELPER_H

#include <llvm/IR/Value.h>

#include "TokenInfo.h"
#include "EntryPoint.h"

class BaseValue;
class Type;
class UserTypeValue;

struct TokenInfo;

struct ReadProxy
{
	const EntryPoint entryPoint;
	const BaseValue *value;
};

class InteropHelper
{
	public:
		static ReadProxy createReadProxy(llvm::Value *value
			, const Type *type
			, const EntryPoint &entryPoint
			, const std::shared_ptr<peg::Ast> &ast
			);

		static llvm::Value *createWriteProxy(const UserTypeValue *userTypeValue, const EntryPoint &entryPoint);
};

#endif // INTEROPHELPER_H
