#ifndef STRINGOPERATORSET_H
#define STRINGOPERATORSET_H

#include "IOperatorSet.h"

class StringOperatorSet : public IOperatorSet
{
	public:
		fragment* forBinary(const fluenc::binary_node *node) const override;
		fragment* forUnary(const fluenc::unary_node *node) const override;
};

#endif // STRINGOPERATORSET_H
