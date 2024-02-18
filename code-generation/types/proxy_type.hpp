#pragma once

#include "type.hpp"

namespace fluenc::code_generation::types
{
	struct proxy_type : public base_type
	{
		proxy_type(llvm::Type* type);

		type_id id() const override;

		std::string name() const override;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;

	private:
		llvm::Type* type_;
	};
}
