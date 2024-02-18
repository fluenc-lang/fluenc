#pragma once

#include "Type.h"

class proxy_type : public Type
{
	public:
		proxy_type(llvm::Type *type);

                type_id id() const override;

		std::string name() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

	private:
		llvm::Type *m_type;
};
