#pragma once

#include "types/IOperatorSet.h"

class FloatOperatorSet : public IOperatorSet
{
	public:
		fragment* forBinary(const fluenc::binary_node *node) const override;
		fragment* forUnary(const fluenc::unary_node *node) const override;
};
