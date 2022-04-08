#ifndef EXTREMITIESITERATOR_H
#define EXTREMITIESITERATOR_H

#include <cmath>

template<typename T>
class extremities_iterator
{
	public:
		extremities_iterator(T &min, T &max)
			: m_min(min)
			, m_max(max)
		{
		}

		extremities_iterator &operator ++()
		{
			return *this;
		}

		extremities_iterator &operator *()
		{
			return *this;
		}

		extremities_iterator &operator =(T value)
		{
			m_min = std::min(m_min, value);
			m_max = std::max(m_max, value);

			return *this;
		}

	private:
		T &m_min;
		T &m_max;
};

#endif // EXTREMITIESITERATOR_H
