#include "types/BooleanOperatorSet.h"

#include "nodes/BinaryNode.h"
#include "nodes/UnaryNode.h"

const Node *BooleanOperatorSet::forBinary(const BinaryNode *node) const
{
	auto result = new BooleanBinaryNode();
	result->ast = node->ast;
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}

const Node *BooleanOperatorSet::forUnary(const UnaryNode *node) const
{
	auto result = new BooleanUnaryNode();
	result->ast = node->ast;
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}
