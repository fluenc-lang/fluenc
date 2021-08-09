#ifndef STACK_H
#define STACK_H

#include <llvm/IR/Value.h>

#include <stack>

class Stack
{
	public:
		Stack() = default;
		Stack(const Stack &other) = default;

		std::vector<llvm::Value *, std::allocator<llvm::Value *>>::const_iterator begin() const;
		std::vector<llvm::Value *, std::allocator<llvm::Value *>>::const_iterator end() const;

		llvm::Value *pop();

		void push(llvm::Value *value);

	private:
		std::vector<llvm::Value *> m_values;
};

#endif // STACK_H
