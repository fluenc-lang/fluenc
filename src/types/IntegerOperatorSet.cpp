#include "types/IntegerOperatorSet.h"

#include "nodes/TypedOperatorNode.h"

const Node *IntegerOperatorSet::forBinary(const BinaryNode *node) const
{
	return new TypedOperatorNode<BinaryNode, &NodeVisitor::visitIntegerBinary>(node);
}
