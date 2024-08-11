#pragma once

#include <string>

#include "type.hpp"

namespace fluenc
{
	struct struct_node;

	namespace code_generation::types
	{
		struct structure_field
		{
			std::string name;

			const base_type* type;
		};

		struct structure_type : public base_type
		{
			structure_type(const struct_node* subject);

			type_id id() const override;

			std::string name() const override;

			llvm::Type* storage_type(llvm::LLVMContext& context) const override;

			std::vector<structure_field> fields(const entry_point& type_resolver) const;

			const struct_node* subject() const;

			static structure_type* get(const struct_node* subject);

		private:
			const struct_node* subject_;
		};
	}
}
