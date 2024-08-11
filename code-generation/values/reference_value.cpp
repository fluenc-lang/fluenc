#include "reference_value.hpp"

#include "entry_point.hpp"
#include "ir_builder.hpp"

namespace fluenc::code_generation::values
{
	reference_value::reference_value(const base_type* type, llvm::Value* value)
		: type_(type)
		, value_(value)
	{
	}

	value_id reference_value::id() const
	{
		return value_id::reference;
	}

	const base_type* reference_value::type() const
	{
		return type_;
	}

	const base_value* reference_value::clone(const entry_point& entry_point, clone_strategy strategy) const
	{
		auto alloc = entry_point.alloc(type_);

		if (strategy == clone_strategy::value)
		{
			ir_builder builder(entry_point);

			auto load = builder.create_load(this);

			builder.create_store(load, alloc);
		}

		return alloc;
	}

	reference_value::operator llvm::Value*() const
	{
		return value_;
	}
}
