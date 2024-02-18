#pragma once

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

#include "base_value.hpp"

#include "metadata/scalar_value_metadata.hpp"

struct base_type;

namespace fluenc::code_generation::values
{
	struct scalar_value : public base_value_with_metadata<scalar_value_metadata>
	{
		scalar_value(const base_type* type, llvm::Value* value);

		value_id id() const override;

		const base_type* type() const override;
		const base_value* clone(const entry_point& entryPoint, clone_strategy strategy) const override;

		llvm::Constant* constant() const;

		operator llvm::Value*() const;

	private:
		const base_type* type_;

		llvm::Value* value_;
	};
}
