#include "types/IntegerOperatorSet.h"

#include "nodes/BinaryNode.h"
#include "nodes/UnaryNode.h"

const Node *IntegerOperatorSet::forBinary(const BinaryNode *node) const
{
	auto result = new IntegerBinaryNode();
	result->ast = node->ast;
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}

const Node *IntegerOperatorSet::forUnary(const UnaryNode *node) const
{
	auto result = new IntegerUnaryNode();
	result->ast = node->ast;
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}
