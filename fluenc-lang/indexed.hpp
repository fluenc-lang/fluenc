#pragma once

#include <cstddef>

namespace fluenc
{
	template <typename T>
	struct indexed
	{
		size_t index;

		T value;
	};
}
