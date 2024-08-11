#pragma once

#include "integer_type.hpp"

namespace fluenc::code_generation::types
{
	struct byte_type : public integer_type<byte_type>
	{
		std::string name() const override;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;
	};
}
