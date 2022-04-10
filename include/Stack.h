#ifndef STACK_H
#define STACK_H

#include <llvm/IR/Value.h>

#include <stack>
#include <vector>

class BaseValue;

class Stack
{
	public:
		typedef std::vector<const BaseValue *, std::allocator<const BaseValue *>>::const_iterator const_iterator;
		typedef std::vector<const BaseValue *, std::allocator<const BaseValue *>>::const_reverse_iterator const_reverse_iterator;

		Stack() = default;
		Stack(const std::vector<const BaseValue *> &values);
		Stack(const BaseValue *value);
		Stack(const Stack &other) = default;

		size_t size() const;

		const_iterator begin() const;
		const_iterator end() const;

		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend() const;

		const BaseValue *pop();

		Stack &discard()
		{
			pop();

			return *this;
		}

		template<typename T>
		const T *require()
		{
			auto value = pop();

			if (auto casted = dynamic_cast<const T *>(value))
			{
				return casted;
			}

			throw new std::exception();
		}

		template<typename T>
		const T *request()
		{
			if (size() <= 0)
			{
				return nullptr;
			}

			auto value = pop();

			if (auto casted = dynamic_cast<const T *>(value))
			{
				return casted;
			}

			return nullptr;
		}

		void push(const BaseValue *value);

	private:
		std::vector<const BaseValue *> m_values;
};

#endif // STACK_H
