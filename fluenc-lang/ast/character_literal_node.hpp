#pragma once

#include <string>

#include "expression.hpp"

namespace fluenc
{
	struct character_literal_node
	{
		expression_t consumer;

		std::string value;
	};
}
