#pragma once

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
