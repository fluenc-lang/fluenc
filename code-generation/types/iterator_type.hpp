#pragma once

#include "type.hpp"

namespace fluenc::code_generation::types
{
	struct iterator_type : public base_type
	{
		static base_type* instance();

		type_id id() const override;

		std::string name() const override;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;

		static base_type* get(uint64_t it);
	};
}
