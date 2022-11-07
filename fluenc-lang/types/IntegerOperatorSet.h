#ifndef INTEGEROPERATORSET_H
#define INTEGEROPERATORSET_H

#include "IOperatorSet.h"

class IntegerOperatorSet : public IOperatorSet
{
	public:
		const Node *forBinary(const BinaryNode *node) const override;
		const Node *forUnary(const UnaryNode *node) const override;
};

#endif // INTEGEROPERATORSET_H
