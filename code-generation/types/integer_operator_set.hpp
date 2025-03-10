#pragma once

#include "operator_set.hpp"

namespace fluenc::code_generation::types
{
	struct integer_operator_set : public operator_set
	{
		fragment* for_binary(const binary_node* node) const override;
		fragment* for_unary(const unary_node* node) const override;
	};
}
