#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <vector>

template<typename T>
class sequence
{
	public:
		sequence(std::initializer_list<T> init)
			: m_values(init)
		{
		}

		T get()
		{
			return m_values[std::min(m_values.size() - 1, m_index++)];
		}

	private:
		std::size_t m_index;
		std::vector<T> m_values;
};

#endif // SEQUENCE_H
