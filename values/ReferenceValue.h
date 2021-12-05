#ifndef REFERENCEVALUE_H
#define REFERENCEVALUE_H

#include <llvm/IR/Value.h>

#include "BaseValue.h"

class ReferenceValue : public BaseValue
{
	public:
		ReferenceValue(const Type *type, llvm::Value *value);

		const Type *type() const override;

		operator llvm::Value *() const;

	private:
		const Type *m_type;

		llvm::Value *m_value;
};

#endif // REFERENCEVALUE_H
