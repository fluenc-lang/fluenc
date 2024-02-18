#pragma once

#include <vector>

#ifndef DEBUG
#include "immer/vector.hpp"
#endif

#include "exceptions/invalid_type_exception.hpp"

#include "base_value.hpp"

namespace fluenc::code_generation
{
	class value_stack
	{
	private:
#ifndef DEBUG
		immer::vector<const BaseValue*> values_;
#else
		std::vector<const base_value*> values_;
#endif

	public:
		using const_iterator = decltype(values_)::const_iterator;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		value_stack() = default;

		value_stack(const std::vector<const base_value*>& values);
		value_stack(const base_value* value);
		value_stack(const value_stack& other) = default;

		size_t size() const;

		const_iterator begin() const;
		const_iterator end() const;

		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend() const;

		const base_value* pop();
		const base_value* peek();

		value_stack& discard();

		template <typename TValue>
		const TValue* require(const std::shared_ptr<peg::Ast>& ast)
		{
			auto value = pop();

			if (auto casted = value_cast<const TValue*>(value))
			{
				return casted;
			}

			auto& expectedMetadata = TValue::static_metadata();
			auto& actualMetadata = value->metadata();

			throw invalid_type_exception(ast, expectedMetadata.name(), actualMetadata.name());
		}

		template <typename TValue>
		const TValue* request()
		{
			if (size() <= 0)
			{
				return nullptr;
			}

			auto value = pop();

			if (auto casted = value_cast<const TValue*>(value))
			{
				return casted;
			}

			return nullptr;
		}

		void push(const base_value* value);
	};
}
