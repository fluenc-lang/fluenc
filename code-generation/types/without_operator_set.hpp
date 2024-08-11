#pragma once

#include "operator_set.hpp"

namespace fluenc::code_generation::types
{
	struct without_operator_set : public operator_set
	{
		fragment* for_binary(const binary_node* node) const;
		fragment* for_unary(const unary_node* node) const;
	};
}
