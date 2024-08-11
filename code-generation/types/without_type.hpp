#pragma once

#include "builtin_type.hpp"

namespace fluenc::code_generation::types
{
	struct without_type : public builtin_type<without_type>
	{
		type_id id() const override;

		std::string name() const override;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;

		operator_set* operators() const override;
	};
}
