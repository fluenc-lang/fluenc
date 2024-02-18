#include "named_value.hpp"

namespace fluenc::code_generation::values
{
	named_value::named_value(const std::string& name, const base_value* value)
		: name_(name)
		, value_(value)
	{
	}

	value_id named_value::id() const
	{
		return value_id::named;
	}

	std::string named_value::name() const
	{
		return name_;
	}

	const base_value* named_value::value() const
	{
		return value_;
	}

	const base_type* named_value::type() const
	{
		return value_->type();
	}

	const base_value* named_value::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		return new named_value(name_, value_->clone(entryPoint, strategy));
	}
}
