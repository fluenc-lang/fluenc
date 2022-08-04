#include "types/BooleanOperatorSet.h"

#include "nodes/TypedOperatorNode.h"

const Node *BooleanOperatorSet::forBinary(const BinaryNode *node) const
{
	return new TypedOperatorNode<BinaryNode, &NodeVisitor::visitBooleanBinary>(node);
}
