#include "utility.hpp"
#include "without_value.hpp"

#include "types/without_type.hpp"

namespace fluenc::code_generation::values
{
	const without_value* without_value::instance()
	{
		static without_value instance;

		return &instance;
	}

	value_id without_value::id() const
	{
		return value_id::without;
	}

	const base_type* without_value::type() const
	{
		return types::without_type::instance();
	}

	const base_value* without_value::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		UNUSED(entryPoint);
		UNUSED(strategy);

		return this;
	}
}
