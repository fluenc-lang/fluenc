#ifndef INDEXITERATOR_H
#define INDEXITERATOR_H

class index_iterator
{
	public:
		index_iterator();

		index_iterator &operator ++();
		index_iterator &operator =(int value);

		int operator *();

	private:
		int m_index;
};

#endif // INDEXITERATOR_H
