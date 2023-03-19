#ifndef WITHOUTOPERATORSET_H
#define WITHOUTOPERATORSET_H

#include "IOperatorSet.h"

class WithoutOperatorSet : public IOperatorSet
{
	public:
		const Node *forBinary(const BinaryNode *node) const;
		const Node *forUnary(const UnaryNode *node) const;
};

#endif // WITHOUTOPERATORSET_H
