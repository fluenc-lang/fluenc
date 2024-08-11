#include "value_stack.hpp"

namespace fluenc::code_generation
{
	value_stack::value_stack(const std::vector<const base_value*>& values)
		: values_(std::begin(values), std::end(values))
	{
	}

	value_stack::value_stack(const base_value* value)
		: values_({ value })
	{
	}

	size_t value_stack::size() const
	{
		return values_.size();
	}

	value_stack::const_iterator value_stack::begin() const
	{
		return values_.begin();
	}

	value_stack::const_iterator value_stack::end() const
	{
		return values_.end();
	}

	value_stack::const_reverse_iterator value_stack::rbegin() const
	{
		return values_.rbegin();
	}

	value_stack::const_reverse_iterator value_stack::rend() const
	{
		return values_.rend();
	}

	const base_value* value_stack::pop()
	{
		if (values_.empty())
		{
			throw std::exception();
		}

		auto value = values_.back();

#ifndef DEBUG
		values_ = values_.take(values_.size() - 1);
#else
		values_.pop_back();
#endif

		return value;
	}

	const base_value* value_stack::peek()
	{
		return values_.back();
	}

	value_stack& value_stack::discard()
	{
		pop();

		return *this;
	}

	void value_stack::push(const base_value* value)
	{
#ifndef DEBUG
		values_ = values_.push_back(value);
#else
		values_.push_back(value);
#endif
	}
}
