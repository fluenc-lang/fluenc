#include "expanded_lazy_value.hpp"
#include "pre_lazy_value.hpp"

namespace fluenc::code_generation::values
{
	expanded_lazy_value::expanded_lazy_value(const pre_lazy_value* subject)
		: subject_(subject)
	{
	}

	value_id expanded_lazy_value::id() const
	{
		return value_id::expanded_lazy;
	}

	const base_type* expanded_lazy_value::type() const
	{
		return subject_->type();
	}

	const base_value* expanded_lazy_value::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		auto cloned = subject_->clone(entryPoint, strategy);
		auto casted = reinterpret_cast<const pre_lazy_value*>(cloned);

		return new expanded_lazy_value(casted);
	}

	const pre_lazy_value* expanded_lazy_value::subject() const
	{
		return subject_;
	}
}
