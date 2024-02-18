#include "placeholder_value.hpp"
#include "utility.hpp"

#include "types/placeholder_type.hpp"

namespace fluenc::code_generation::values
{
	placeholder_value* placeholder_value::instance()
	{
		static placeholder_value instance;

		return &instance;
	}

	value_id placeholder_value::id() const
	{
		return value_id::placeholder;
	}

	const base_type* placeholder_value::type() const
	{
		static types::placeholder_type type;

		return &type;
	}

	const base_value* placeholder_value::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		UNUSED(entryPoint);
		UNUSED(strategy);

		return this;
	}
}
