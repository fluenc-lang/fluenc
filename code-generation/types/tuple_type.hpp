#pragma once

#include <vector>

#include "type.hpp"

namespace fluenc::code_generation::types
{
	struct tuple_type : public base_type
	{
		tuple_type(const std::vector<const base_type*> types);

		type_id id() const override;

		std::string name() const override;
		std::string full_name() const override;

		std::vector<const base_type*> types() const;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;

		static tuple_type* get(const std::vector<const base_type*>& types);

	private:
		std::vector<const base_type*> types_;
	};
}
