#include "indexed_value.hpp"

namespace fluenc::code_generation::values
{
	indexed_value::indexed_value(size_t index, const base_value* subject)
		: index_(index)
		, subject_(subject)
	{
	}

	value_id indexed_value::id() const
	{
		return value_id::indexed;
	}

	size_t indexed_value::index() const
	{
		return index_;
	}

	const base_type* indexed_value::type() const
	{
		return subject_->type();
	}

	const base_value* indexed_value::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		return new indexed_value(index_, subject_->clone(entryPoint, strategy));
	}

	const base_value* indexed_value::subject() const
	{
		return subject_;
	}
}
