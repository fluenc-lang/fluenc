#pragma once

#include "operator_set.hpp"

namespace fluenc::code_generation::types
{
	struct boolean_operator_set : public operator_set
	{
		fragment* for_binary(const fluenc::binary_node* node) const override;
		fragment* for_unary(const fluenc::unary_node* node) const override;
	};
}
