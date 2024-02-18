#pragma once

#include "BuiltinType.h"

class void_type : public BuiltinType<void_type>
{
	public:
        type_id id() const override;

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};
