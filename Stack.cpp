#include "Stack.h"

size_t Stack::size() const
{
	return m_values.size();
}

std::vector<TypedValue, std::allocator<TypedValue>>::const_iterator Stack::begin() const
{
	return m_values.begin();
}

std::vector<TypedValue, std::allocator<TypedValue>>::const_iterator Stack::end() const
{
	return m_values.end();
}

std::vector<TypedValue, std::allocator<TypedValue>>::const_reverse_iterator Stack::rbegin() const
{
	return m_values.rbegin();
}

std::vector<TypedValue, std::allocator<TypedValue>>::const_reverse_iterator Stack::rend() const
{
	return m_values.rend();
}

TypedValue Stack::pop()
{
	auto value = m_values.back();

	m_values.pop_back();

	return value;
}

void Stack::push(const TypedValue &value)
{
	m_values.push_back(value);
}
