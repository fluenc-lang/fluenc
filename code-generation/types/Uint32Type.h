#pragma once

#include "IntegerType.h"

class uint32_type : public IntegerType<uint32_type>
{
	public:
		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;
};
