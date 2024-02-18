#include "all_iterator.hpp"

all_true::all_true(bool& value)
	: m_value(value)
{
}

bool all_true::value() const
{
	return m_value;
}

all_true& all_true::operator++()
{
	return *this;
}

all_true& all_true::operator*()
{
	return *this;
}

all_true& all_true::operator=(bool value)
{
	m_value &= value;

	return *this;
}
