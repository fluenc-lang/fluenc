#ifndef REFERENCEVALUE_H
#define REFERENCEVALUE_H

#include <llvm/IR/Value.h>

#include "BaseValue.h"

#include "metadata/ReferenceValueMetadata.h"

class ReferenceValue : public BaseValueWithMetadata<ReferenceValueMetadata>
{
	public:
		ReferenceValue(const Type *type, llvm::Value *value);

		const Type *type() const override;
		const BaseValue *clone(const EntryPoint &entryPoint, CloneStrategy strategy) const override;

		operator llvm::Value *() const;

	private:
		const Type *m_type;

		llvm::Value *m_value;
};

#endif // REFERENCEVALUE_H
