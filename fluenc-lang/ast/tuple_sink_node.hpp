#pragma once

#include "expression.hpp"

namespace fluenc
{
	struct tuple_sink_node
	{
		size_t size;
		expression_t consumer;
	};
}
