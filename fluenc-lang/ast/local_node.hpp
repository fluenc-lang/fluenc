#pragma once

#include <string>

#include "expression.hpp"

namespace fluenc
{
	struct local_node
	{
		expression_t consumer;

		std::string name;
	};
}
