#include "forwarded_value.hpp"

namespace fluenc::code_generation::values
{
	forwarded_value::forwarded_value(const base_value* subject)
		: subject_(subject)
	{
	}

	value_id forwarded_value::id() const
	{
		return value_id::forwarded;
	}

	const base_value* forwarded_value::subject() const
	{
		return subject_;
	}

	const base_type* forwarded_value::type() const
	{
		return subject_->type();
	}

	const base_value* forwarded_value::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		auto subject = subject_->clone(entryPoint, strategy);

		return new forwarded_value(subject);
	}

	const value_metadata& forwarded_value::metadata() const
	{
		return subject_->metadata();
	}
}
