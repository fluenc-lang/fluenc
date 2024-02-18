#pragma once

#include "expression.hpp"

struct iterator;

namespace fluenc
{
	struct return_node
	{
		const iterator* iterator_; // <-

		expression_t consumer;

		std::optional<expression_t> chained;
	};
}
