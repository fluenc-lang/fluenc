#pragma once

#include "IOperatorSet.h"

class ArrayOperatorSet : public IOperatorSet
{
	public:
		fragment* forBinary(const fluenc::binary_node *node) const;
		fragment* forUnary(const fluenc::unary_node *node) const;
};
