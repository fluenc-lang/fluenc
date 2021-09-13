#ifndef STACK_H
#define STACK_H

#include <llvm/IR/Value.h>

#include <stack>

#include "TypedValue.h"

class Stack
{
	public:
		Stack() = default;
		Stack(const Stack &other) = default;

		std::vector<TypedValue, std::allocator<TypedValue>>::const_iterator begin() const;
		std::vector<TypedValue, std::allocator<TypedValue>>::const_iterator end() const;

		std::vector<TypedValue, std::allocator<TypedValue>>::const_reverse_iterator rbegin() const;
		std::vector<TypedValue, std::allocator<TypedValue>>::const_reverse_iterator rend() const;

		TypedValue pop();

		void push(const TypedValue &value);

	private:
		std::vector<TypedValue> m_values;
};

#endif // STACK_H
