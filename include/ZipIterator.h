#ifndef ZIPITERATOR_H
#define ZIPITERATOR_H

#include <tuple>

template<typename T1, typename T2>
class zip_iterator
{
	public:
		zip_iterator(T1 left, T2 right)
			: m_left(left)
			, m_right(right)
		{
		}

		zip_iterator &operator ++()
		{
			m_left++;
			m_right++;

			return *this;
		}

		auto operator *()
		{
			return std::make_tuple(*m_left, *m_right);
		}

		bool operator !=(const zip_iterator &other)
		{
			return m_left != other.m_left
				&& m_right != other.m_right;
		}

	private:
		T1 m_left;
		T2 m_right;
};

template<typename T1, typename T2>
class zip_container
{
	public:
		typedef zip_iterator<typename std::decay_t<T1>::const_iterator, typename std::decay_t<T2>::const_iterator> iterator;

		zip_container(const T1 &left, const T2 &right)
			: m_left(left)
			, m_right(right)
		{
		}

		iterator begin() const
		{
			return iterator(m_left.begin(), m_right.begin());
		}

		iterator end() const
		{
			return iterator(m_left.end(), m_right.end());
		}

	private:
		const T1 &m_left;
		const T2 &m_right;
};

template<typename ...T>
zip_container<T...> zip(const T &... subjects)
{
	return { subjects... };
}

#endif // ZIPITERATOR_H
