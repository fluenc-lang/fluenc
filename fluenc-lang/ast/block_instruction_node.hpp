#pragma once

#include "expression.hpp"

namespace fluenc
{
	struct block_instruction_node
	{
		expression_t subject;

		bool contains_iterator;
	};
}
