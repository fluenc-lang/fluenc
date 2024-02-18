#pragma once

#include <cstddef>
#include <iterator>

namespace fluenc
{
	class index_iterator
	{
	public:
		index_iterator();
		index_iterator(size_t index);

		index_iterator& operator++();
		index_iterator& operator--();
		index_iterator& operator=(size_t value);

		size_t operator*();

		bool operator!=(const index_iterator& other);

	private:
		size_t m_index;
	};
}

namespace std
{
	template <>
	struct iterator_traits<fluenc::index_iterator>
	{
		using difference_type = size_t;
		using value_type = size_t;
		using pointer = size_t*;
		using reference = const size_t&;
		using iterator_category = bidirectional_iterator_tag;
	};
}
