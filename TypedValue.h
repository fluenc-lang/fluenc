#ifndef TYPEDVALUE_H
#define TYPEDVALUE_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

class Type;

class TypedValue
{
	public:
		TypedValue();
		TypedValue(Type *type, llvm::Value *value);

		Type *type() const;

		llvm::Constant *constant() const;

		operator llvm::Value *() const;

	private:
		Type *m_type;

		llvm::Value *m_value;
};

#endif // TYPEDVALUE_H
