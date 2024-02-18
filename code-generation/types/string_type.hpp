#pragma once

#include "builtin_type.hpp"

namespace fluenc::code_generation::types
{
	struct string_type : public builtin_type<string_type>
	{
		string_type();
		string_type(size_t length);

		type_id id() const override;

		size_t length() const;

		std::string name() const override;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;

		operator_set* operators() const override;

	private:
		size_t length_;
	};
}
