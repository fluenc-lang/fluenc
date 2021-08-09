#ifndef STACK_H
#define STACK_H

#include <llvm/IR/Value.h>

#include <stack>

class Stack
{
	public:
		Stack() = default;
		Stack(const Stack &other) = default;

		llvm::Value *pop();

		void push(llvm::Value *value);

	private:
		std::stack<llvm::Value *> m_values;
};

#endif // STACK_H
