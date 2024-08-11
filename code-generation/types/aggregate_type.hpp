#pragma once

#include "type.hpp"

namespace fluenc::code_generation::types
{
	struct aggregate_type : public base_type
	{
		aggregate_type(const std::vector<const base_type*>& subjects);

		type_id id() const override;

		std::string name() const override;
		std::string full_name() const override;

		std::vector<const base_type*> subjects() const;

		llvm::Type* storage_type(llvm::LLVMContext& context) const override;

		static aggregate_type* get(const std::vector<const base_type*>& types);

	private:
		const std::vector<const base_type*> subjects_;
	};
}
