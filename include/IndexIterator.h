#ifndef INDEXITERATOR_H
#define INDEXITERATOR_H

#include <cstddef>

class index_iterator
{
	public:
		index_iterator();
		index_iterator(size_t index);

		index_iterator &operator ++();
		index_iterator &operator =(size_t value);

		size_t operator *();

		bool operator !=(const index_iterator &other);

	private:
		size_t m_index;
};

#endif // INDEXITERATOR_H
