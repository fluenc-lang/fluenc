#include "function_node.hpp"
#include "ast.hpp"

namespace fluenc
{
	function_attribute attribute(const function_node* node)
	{
		if (auto block = ast_cast<block_instruction_node>(node->block))
		{
			if (block->contains_iterator)
			{
				return function_attribute::iterator;
			}
		}

		return function_attribute::none;
	}
}
