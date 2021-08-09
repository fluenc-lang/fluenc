#include "Stack.h"

llvm::Value *Stack::pop()
{
	auto value = m_values.top();

	m_values.pop();

	return value;
}

void Stack::push(llvm::Value *value)
{
	m_values.push(value);
}
