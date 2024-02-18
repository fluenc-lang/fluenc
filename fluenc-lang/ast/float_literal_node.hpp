#pragma once

#include "expression.hpp"
#include "type_name_node.hpp"

namespace fluenc
{
	struct float_literal_node
	{
		expression_t consumer;
		type_name_t type;

		std::string value;
	};
}
