#ifndef SCALARVALUE_H
#define SCALARVALUE_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

#include "values/BaseValue.h"

class Type;

class ScalarValue : public BaseValue
{
	public:
		ScalarValue(const Type *type, llvm::Value *value);

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint) const override;

		llvm::Constant *constant() const;

		operator llvm::Value *() const;

	private:
		const Type *m_type;

		llvm::Value *m_value;
};

#endif // SCALARVALUE_H
