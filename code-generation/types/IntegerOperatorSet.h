#pragma once

#include "IOperatorSet.h"

class IntegerOperatorSet : public IOperatorSet
{
	public:
		fragment* forBinary(const fluenc::binary_node *node) const override;
		fragment* forUnary(const fluenc::unary_node *node) const override;
};
