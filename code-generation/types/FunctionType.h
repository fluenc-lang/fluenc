#pragma once

#include "Type.h"

#include "expression.hpp"

class function_type : public Type
{
	public:
		function_type(const std::vector<const Type *> &types, fluenc::function_node *function);

		type_id id() const override;

		std::string name() const override;
		std::string fullName() const override;

		std::vector<const Type *> types() const;

		fluenc::function_node* function() const;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

				static function_type *get(const std::vector<const Type *> &types, fluenc::function_node *function);

	private:
		std::vector<const Type *> m_types;

		fluenc::function_node* m_function;
};
