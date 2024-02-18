#pragma once

#include "type.hpp"

namespace fluenc::code_generation::types
{
	struct placeholder_type : public base_type
	{
		type_id id() const override;

		std::string name() const override;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;
	};
}
