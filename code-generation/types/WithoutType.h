#pragma once

#include "BuiltinType.h"

class without_type : public BuiltinType<without_type>
{
	public:
        type_id id() const override;

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		IOperatorSet *operators() const override;
};
