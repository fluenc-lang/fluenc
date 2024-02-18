#ifndef SCALARVALUE_H
#define SCALARVALUE_H

#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>

#include "BaseValue.h"

#include "metadata/ScalarValueMetadata.h"

class Type;

class scalar_value : public BaseValueWithMetadata<ScalarValueMetadata>
{
	public:
		scalar_value(const Type *type, llvm::Value *value);

                value_id id() const override;

		const Type *type() const override;
                const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

		llvm::Constant *constant() const;

		operator llvm::Value *() const;

	private:
		const Type *m_type;

		llvm::Value *m_value;
};

#endif // SCALARVALUE_H
