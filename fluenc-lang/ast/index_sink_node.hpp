#pragma once

#include "expression.hpp"

namespace fluenc
{
	struct index_sink_node
	{
		size_t index;

		expression_t consumer;
	};
}
