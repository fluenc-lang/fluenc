#ifndef STRINGOPERATORSET_H
#define STRINGOPERATORSET_H

#include "IOperatorSet.h"

class StringOperatorSet : public IOperatorSet
{
	public:
		const Node *forBinary(const BinaryNode *node) const override;
		const Node *forUnary(const UnaryNode *node) const override;
};

#endif // STRINGOPERATORSET_H
