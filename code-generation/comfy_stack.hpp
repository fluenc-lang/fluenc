#pragma once

#include <vector>

#ifndef DEBUG
#include "immer/vector.hpp"
#endif

namespace fluenc::code_generation
{
	template <typename T>
	class comfy_stack
	{
	private:
#ifndef DEBUG
		immer::vector<T> values_;
#else
		std::vector<T> values_;
#endif

	public:
		using const_iterator = decltype(values_)::const_iterator;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		comfy_stack() = default;

		comfy_stack(const std::vector<T>& values)
			: values_(std::begin(values), std::end(values))
		{
		}

		comfy_stack(T value)
			: values_({ value })
		{
		}

		comfy_stack(const comfy_stack& other) = default;

		size_t size() const
		{
			return values_.size();
		}

		const_iterator begin() const
		{
			return values_.begin();
		}

		const_iterator end() const
		{
			return values_.end();
		}

		const_reverse_iterator rbegin() const
		{
			return values_.rbegin();
		}

		const_reverse_iterator rend() const
		{
			return values_.rend();
		}

		T pop()
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

		T peek()
		{
			return values_.back();
		}

		comfy_stack& discard()
		{
			pop();

			return *this;
		}

		void push(T value)
		{
#ifndef DEBUG
			values_ = values_.push_back(value);
#else
			values_.push_back(value);
#endif
		}
	};
}
