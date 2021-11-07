#include "Stack.h"

size_t Stack::size() const
{
	return m_values.size();
}

std::vector<const BaseValue *, std::allocator<const BaseValue *>>::const_iterator Stack::begin() const
{
	return m_values.begin();
}

std::vector<const BaseValue *, std::allocator<const BaseValue *>>::const_iterator Stack::end() const
{
	return m_values.end();
}

std::vector<const BaseValue *, std::allocator<const BaseValue *>>::const_reverse_iterator Stack::rbegin() const
{
	return m_values.rbegin();
}

std::vector<const BaseValue *, std::allocator<const BaseValue *>>::const_reverse_iterator Stack::rend() const
{
	return m_values.rend();
}

const BaseValue *Stack::pop()
{
	auto value = m_values.back();

	m_values.pop_back();

	return value;
}

void Stack::push(const BaseValue *value)
{
	m_values.push_back(value);
}
