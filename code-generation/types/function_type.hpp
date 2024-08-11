#pragma once

#include "type.hpp"
#include "expression.hpp"

namespace fluenc::code_generation::types
{
	struct function_type : public base_type
	{
		function_type(const std::vector<const base_type*>& types, function_node* function);

		type_id id() const override;

		std::string name() const override;
		std::string full_name() const override;

		std::vector<const base_type*> types() const;

		function_node* function() const;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;

		static function_type* get(const std::vector<const base_type*>& types, function_node* function);

	private:
		std::vector<const base_type*> types_;

		function_node* function_;
	};
}
