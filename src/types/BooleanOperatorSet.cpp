#include "types/BooleanOperatorSet.h"

#include "nodes/BinaryNode.h"

const Node *BooleanOperatorSet::forBinary(const BinaryNode *node) const
{
	return new BooleanBinaryNode(node);
}
