#pragma once

#include "type.hpp"

namespace fluenc::code_generation::types
{
	struct structure_type;

	struct user_type : public base_type
	{
		user_type(const structure_type* prototype, const std::vector<const base_type*>& elementTypes);

		type_id id() const override;

		std::string name() const override;
		std::string full_name() const override;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;

		const structure_type* prototype() const;

		operator_set* operators() const override;

		std::vector<const base_type*> element_types() const;

		static user_type* get(const structure_type* prototype, const std::vector<const base_type*>& elementTypes);

	private:
		const structure_type* prototype_;

		std::vector<const base_type*> element_types_;
	};
}
