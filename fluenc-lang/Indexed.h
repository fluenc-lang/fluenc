#ifndef INDEXED_H
#define INDEXED_H

#include <cstddef>

class Node;

template <typename T>
struct Indexed
{
	size_t index;

	T value;
};

#endif // INDEXED_H
