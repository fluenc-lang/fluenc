#ifndef TYPEDVALUE_H
#define TYPEDVALUE_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

#include "values/BaseValue.h"

class Type;

class TypedValue : public BaseValue
{
	public:
		TypedValue(const Type *type, llvm::Value *value);

		const Type *type() const;

		llvm::Constant *constant() const;

		operator llvm::Value *() const;

	private:
		const Type *m_type;

		llvm::Value *m_value;
};

#endif // TYPEDVALUE_H
