#pragma once

#include "type.hpp"

namespace fluenc::code_generation::types
{
	struct structure_type;

	struct opaque_pointer_type : public base_type
	{
		opaque_pointer_type(const structure_type* subject);

		type_id id() const override;

		std::string name() const override;
		std::string full_name() const override;

		const structure_type* subject() const;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;

		static opaque_pointer_type* get(const structure_type* subject);

	private:
		const structure_type* subject_;
	};
}
