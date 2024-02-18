#pragma once

#include "Type.h"

class any_type : public Type
{
	public:
		static Type *instance();

                type_id id() const override;

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};
