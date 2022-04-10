#include "IndexIterator.h"

index_iterator::index_iterator()
	: m_index(0)
{
}

index_iterator::index_iterator(size_t index)
	: m_index(index)
{
}

index_iterator &index_iterator::operator ++()
{
	m_index++;

	return *this;
}

index_iterator &index_iterator::operator =(size_t value)
{
	m_index = value;

	return *this;
}

size_t index_iterator::operator *()
{
	return m_index;
}

bool index_iterator::operator !=(const index_iterator &other)
{
	return other.m_index != m_index;
}
