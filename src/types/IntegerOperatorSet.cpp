#include "types/IntegerOperatorSet.h"

#include "nodes/BinaryNode.h"

const Node *IntegerOperatorSet::forBinary(const BinaryNode *node) const
{
	auto result = new IntegerBinaryNode();
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}
