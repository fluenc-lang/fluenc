#include "IndexIterator.h"

index_iterator::index_iterator()
	: m_index(0)
{
}

index_iterator &index_iterator::operator ++()
{
	m_index++;

	return *this;
}

index_iterator &index_iterator::operator =(int value)
{
	m_index = value;

	return *this;
}

int index_iterator::operator *()
{
	return m_index;
}
