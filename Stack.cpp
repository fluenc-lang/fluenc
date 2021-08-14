#include "Stack.h"

std::vector<llvm::Value *, std::allocator<llvm::Value *>>::const_iterator Stack::begin() const
{
	return m_values.begin();
}

std::vector<llvm::Value *, std::allocator<llvm::Value *>>::const_iterator Stack::end() const
{
	return m_values.end();
}

std::vector<llvm::Value *, std::allocator<llvm::Value *>>::const_reverse_iterator Stack::rbegin() const
{
	return m_values.rbegin();
}

std::vector<llvm::Value *, std::allocator<llvm::Value *>>::const_reverse_iterator Stack::rend() const
{
	return m_values.rend();
}

llvm::Value *Stack::pop()
{
	auto value = m_values.back();

	m_values.pop_back();

	return value;
}

void Stack::push(llvm::Value *value)
{
	m_values.push_back(value);
}
