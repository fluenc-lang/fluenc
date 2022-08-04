#include "types/FloatOperatorSet.h"

#include "nodes/TypedOperatorNode.h"

const Node *FloatOperatorSet::forBinary(const BinaryNode *node) const
{
	return new TypedOperatorNode<BinaryNode, &NodeVisitor::visitFloatBinary>(node);
}
