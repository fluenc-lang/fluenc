#ifndef INTEROPHELPER_H
#define INTEROPHELPER_H

#include <llvm/IR/Value.h>

#include "TokenInfo.h"

class BaseValue;
class Type;
class EntryPoint;
class UserTypeValue;

struct TokenInfo;

class InteropHelper
{
	public:
		static const BaseValue *createReadProxy(llvm::Value *value
			, const Type *type
			, const EntryPoint &entryPoint
			, const std::shared_ptr<peg::Ast> &ast
			);

		static llvm::Value *createWriteProxy(const UserTypeValue *userTypeValue, const EntryPoint &entryPoint);
};

#endif // INTEROPHELPER_H
