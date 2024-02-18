#pragma once

#include "type.hpp"

namespace fluenc::code_generation::types
{
	struct array_type : public base_type
	{
		array_type(std::vector<const base_type*> types);

		type_id id() const override;

		std::string name() const override;
		std::string full_name() const override;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;

		operator_set* operators() const override;

		std::vector<const base_type*> types() const;

		static array_type* get(const std::vector<const base_type*>& types);

	private:
		std::vector<const base_type*> types_;
	};
}
