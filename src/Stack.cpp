#include "Stack.h"

Stack::Stack(const std::vector<const BaseValue *> &values)
	: m_values(values)
{
}

Stack::Stack(const BaseValue *value)
	: m_values({ value })
{
}

size_t Stack::size() const
{
	return m_values.size();
}

Stack::const_iterator Stack::begin() const
{
	return m_values.begin();
}

Stack::const_iterator Stack::end() const
{
	return m_values.end();
}

Stack::const_reverse_iterator Stack::rbegin() const
{
	return m_values.rbegin();
}

Stack::const_reverse_iterator Stack::rend() const
{
	return m_values.rend();
}

const BaseValue *Stack::pop()
{
	if (m_values.empty())
	{
		throw new std::exception();
	}

	auto value = m_values.back();

	m_values.pop_back();

	return value;
}

void Stack::push(const BaseValue *value)
{
	m_values.push_back(value);
}
