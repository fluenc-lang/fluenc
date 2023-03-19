#ifndef WITHOUTTYPE_H
#define WITHOUTTYPE_H

#include "BuiltinType.h"

class WithoutType : public BuiltinType<WithoutType>
{
	public:
		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		IOperatorSet *operators() const override;
};

#endif // WITHOUTTYPE_H
