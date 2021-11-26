#ifndef VOIDITERATOR_H
#define VOIDITERATOR_H

template<typename T>
class void_iterator
{
	public:
		void_iterator &operator ++()
		{
			return *this;
		}

		void_iterator &operator *()
		{
			return *this;
		}

		void_iterator &operator =(T)
		{
			return *this;
		}
};

#endif // VOIDITERATOR_H
