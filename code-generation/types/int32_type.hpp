#pragma once

#include "integer_type.hpp"

namespace fluenc::code_generation::types
{
	struct int32_type : public integer_type<int32_type>
	{
		std::string name() const override;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;
	};
}
