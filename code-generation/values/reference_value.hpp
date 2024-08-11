#pragma once

#include <llvm/IR/Value.h>

#include "base_value.hpp"

#include "metadata/reference_value_metadata.hpp"

namespace fluenc::code_generation::values
{
	struct reference_value : public base_value_with_metadata<reference_value_metadata>
	{
		reference_value(const base_type* type, llvm::Value* value);

		value_id id() const override;

		const base_type* type() const override;
		const base_value* clone(const entry_point& entry_point, clone_strategy strategy) const override;

		operator llvm::Value*() const;

	private:
		const base_type* type_;

		llvm::Value* value_;
	};
}
