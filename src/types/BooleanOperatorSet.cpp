#include "types/BooleanOperatorSet.h"

#include "nodes/BinaryNode.h"

const Node *BooleanOperatorSet::forBinary(const BinaryNode *node) const
{
	auto result = new BooleanBinaryNode();
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}
