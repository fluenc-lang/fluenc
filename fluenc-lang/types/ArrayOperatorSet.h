#ifndef ARRAYOPERATORSET_H
#define ARRAYOPERATORSET_H

#include "IOperatorSet.h"

#include "nodes/BinaryNode.h"
#include "nodes/UnaryNode.h"

class ArrayOperatorSet : public IOperatorSet
{
	public:
		const Node *forBinary(const BinaryNode *node) const;
		const Node *forUnary(const UnaryNode *node) const;
};

#endif // ARRAYOPERATORSET_H
