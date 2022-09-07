#include "types/FloatOperatorSet.h"

#include "nodes/BinaryNode.h"

const Node *FloatOperatorSet::forBinary(const BinaryNode *node) const
{
	return new FloatBinaryNode(node);
}
