#pragma once

#include <llvm/IR/Value.h>

#include "BaseValue.h"

#include "metadata/ReferenceValueMetadata.h"

class reference_value : public BaseValueWithMetadata<ReferenceValueMetadata>
{
	public:
		reference_value(const Type *type, llvm::Value *value);

                value_id id() const override;

		const Type *type() const override;
                const BaseValue *clone(const EntryPoint &entryPoint, clone_strategy strategy) const override;

		operator llvm::Value *() const;

	private:
		const Type *m_type;

		llvm::Value *m_value;
};
