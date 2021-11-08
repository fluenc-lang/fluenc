#ifndef STACK_H
#define STACK_H

#include <llvm/IR/Value.h>

#include <stack>

class BaseValue;

class Stack
{
	public:
		Stack() = default;
		Stack(const Stack &other) = default;

		size_t size() const;

		std::vector<const BaseValue *, std::allocator<const BaseValue *>>::const_iterator begin() const;
		std::vector<const BaseValue *, std::allocator<const BaseValue *>>::const_iterator end() const;

		std::vector<const BaseValue *, std::allocator<const BaseValue *>>::const_reverse_iterator rbegin() const;
		std::vector<const BaseValue *, std::allocator<const BaseValue *>>::const_reverse_iterator rend() const;

		const BaseValue *pop();

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

		void push(const BaseValue *value);

	private:
		std::vector<const BaseValue *> m_values;
};

#endif // STACK_H
