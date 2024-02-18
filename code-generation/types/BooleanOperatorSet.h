#ifndef BOOLEANOPERATORSET_H
#define BOOLEANOPERATORSET_H

#include "types/IOperatorSet.h"

class BooleanOperatorSet : public IOperatorSet
{
	public:
		fragment* forBinary(const fluenc::binary_node *node) const override;
		fragment* forUnary(const fluenc::unary_node *node) const override;
};

#endif // BOOLEANOPERATORSET_H
