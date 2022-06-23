#ifndef FLOAT32TYPE_H
#define FLOAT32TYPE_H

#include "BuiltinType.h"

class Float32Type : public BuiltinType<Float32Type>
{
	public:
		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		IOperatorSet *operators() const override;
};

#endif // FLOAT32TYPE_H
