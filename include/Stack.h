#ifndef STACK_H
#define STACK_H

#include <llvm/IR/Value.h>

#include <stack>
#include <vector>

#include "exceptions/InvalidTypeException.h"

#include "values/BaseValue.h"

template<typename T>
class Stack
{
	public:
		using const_iterator = typename std::vector<const T *, std::allocator<const T *>>::const_iterator;
		using const_reverse_iterator = typename std::vector<const T *, std::allocator<const T *>>::const_reverse_iterator;

		Stack() = default;

		Stack(const std::vector<const T *> &values)
			: m_values(values)
		{
		}

		Stack(const T *value)
			: m_values({ value })
		{
		}

		Stack(const Stack &other) = default;

		size_t size() const
		{
			return m_values.size();
		}

		const_iterator begin() const
		{
			return m_values.begin();
		}
		const_iterator end() const
		{
			return m_values.end();
		}

		const_reverse_iterator rbegin() const
		{
			return m_values.rbegin();
		}
		const_reverse_iterator rend() const
		{
			return m_values.rend();
		}

		const T *pop()
		{
			if (m_values.empty())
			{
				throw new std::exception();
			}

			auto value = m_values.back();

			m_values.pop_back();

			return value;
		}

		Stack &discard()
		{
			pop();

			return *this;
		}

		template<typename TValue>
		const TValue *require(const std::shared_ptr<peg::Ast> &ast)
		{
			auto value = pop();

			if (auto casted = dynamic_cast<const TValue *>(value))
			{
				return casted;
			}

			auto &expectedMetadata = TValue::staticMetadata();
			auto &actualMetadata = value->metadata();

			throw new InvalidTypeException(ast
				, expectedMetadata.name()
				, actualMetadata.name()
				);
		}

		template<typename TValue>
		const TValue *request()
		{
			if (size() <= 0)
			{
				return nullptr;
			}

			auto value = pop();

			if (auto casted = dynamic_cast<const TValue *>(value))
			{
				return casted;
			}

			return nullptr;
		}

		void push(const T *value)
		{
			m_values.push_back(value);
		}

	private:
		std::vector<const T *> m_values;
};

#endif // STACK_H
