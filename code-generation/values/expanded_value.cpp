#include "entry_point.hpp"
#include "expanded_value.hpp"
#include "utility.hpp"

namespace fluenc::code_generation::values
{
	expanded_value::expanded_value(bool isArray,
		const base_type* iterator_type,
		const entry_point& provider,
		const fragment* node,
		const std::vector<const expanded_value*>& next,
		const value_stack_t &values
	)
		: is_array_(isArray)
		, iterator_type_(iterator_type)
		, provider_(new entry_point(provider))
		, node_(node)
		, next_(next)
		, values_(values)
	{
	}

	value_id expanded_value::id() const
	{
		return value_id::expanded;
	}

	bool expanded_value::is_array() const
	{
		return is_array_;
	}

	const base_type* expanded_value::type() const
	{
		return iterator_type_;
	}

	const base_value* expanded_value::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		UNUSED(entryPoint);
		UNUSED(strategy);

		return this;
	}

	const entry_point* expanded_value::provider() const
	{
		return provider_;
	}

	const fragment* expanded_value::node() const
	{
		return node_;
	}

	std::vector<const expanded_value*> expanded_value::next() const
	{
		return next_;
	}

	const value_stack_t &expanded_value::values() const
	{
		return values_;
	}
}
