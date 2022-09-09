#include "types/FloatOperatorSet.h"

#include "nodes/BinaryNode.h"

const Node *FloatOperatorSet::forBinary(const BinaryNode *node) const
{
	auto result = new FloatBinaryNode();
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}
