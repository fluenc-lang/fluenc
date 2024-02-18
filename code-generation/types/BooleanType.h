#ifndef BOOLEANTYPE_H
#define BOOLEANTYPE_H

#include "BuiltinType.h"

class boolean_type : public BuiltinType<boolean_type>
{
	public:
		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		IOperatorSet *operators() const override;
};

#endif // BOOLEANTYPE_H
