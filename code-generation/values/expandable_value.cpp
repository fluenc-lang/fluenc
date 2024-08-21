#include "entry_point.hpp"
#include "expandable_value.hpp"
#include "utility.hpp"

#include "types/expanded_type.hpp"

namespace fluenc::code_generation::values
{
	expandable_value::expandable_value(bool is_array,
		const base_type* iterator_type,
		const entry_point& provider,
		const fragment* chain,
		const value_stack_t &values
	)
		: is_array_(is_array)
		, iterator_type_(iterator_type)
		, provider_(new entry_point(provider))
		, chain_(chain)
		, values_(values)
	{
	}

	value_id expandable_value::id() const
	{
		return value_id::expandable;
	}

	bool expandable_value::isArray() const
	{
		return is_array_;
	}

	const base_type* expandable_value::type() const
	{
		return iterator_type_;
	}

	const base_value* expandable_value::clone(const entry_point& entryPoint, clone_strategy strategy) const
	{
		UNUSED(entryPoint);
		UNUSED(strategy);

		return this;
	}

	const entry_point* expandable_value::provider() const
	{
		return provider_;
	}

	const fragment* expandable_value::chain() const
	{
		return chain_;
	}

	const types::expanded_type* expandable_value::expandedType() const
	{
		return type_cast<const types::expanded_type*>(iterator_type_);
	}

	const value_stack_t expandable_value::values() const
	{
		return values_;
	}
}
