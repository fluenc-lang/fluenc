#pragma once

#include "builtin_type.hpp"

namespace fluenc::code_generation::types
{
	struct float32_type : public builtin_type<float32_type>
	{
		std::string name() const override;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;

		operator_set* operators() const override;
	};
}
