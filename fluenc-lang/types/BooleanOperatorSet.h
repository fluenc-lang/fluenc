#ifndef BOOLEANOPERATORSET_H
#define BOOLEANOPERATORSET_H

#include "types/IOperatorSet.h"

class BooleanOperatorSet : public IOperatorSet
{
	public:
		const Node *forBinary(const BinaryNode *node) const override;
		const Node *forUnary(const UnaryNode *node) const override;
};

#endif // BOOLEANOPERATORSET_H
