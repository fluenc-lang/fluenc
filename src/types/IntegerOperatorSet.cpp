#include "types/IntegerOperatorSet.h"

#include "nodes/BinaryNode.h"

const Node *IntegerOperatorSet::forBinary(const BinaryNode *node) const
{
	return new IntegerBinaryNode(node);
}
