#include "entry_point.hpp"
#include "reference_value.hpp"
#include "scalar_value.hpp"

namespace fluenc::code_generation::values
{
	scalar_value::scalar_value(const base_type* type, llvm::Value* value)
		: type_(type)
		, value_(value)
	{
	}

	value_id scalar_value::id() const
	{
		return value_id::scalar;
	}

	const base_type* scalar_value::type() const
	{
		return type_;
	}

	const base_value* scalar_value::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		if (strategy == clone_strategy::storage)
		{
			return entryPoint.alloc(type_);
		}

		return this;
	}

	llvm::Constant* scalar_value::constant() const
	{
		return static_cast<llvm::Constant*>(value_);
	}

	scalar_value::operator llvm::Value*() const
	{
		return value_;
	}
}
