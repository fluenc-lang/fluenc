#pragma once

#include "builtin_type.hpp"

namespace fluenc::code_generation::types
{
	struct void_interop_type : public builtin_type<void_interop_type>
	{
		type_id id() const override;

		std::string name() const override;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;
	};
}
