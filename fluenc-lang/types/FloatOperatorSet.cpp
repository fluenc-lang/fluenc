#include "types/FloatOperatorSet.h"

#include "nodes/BinaryNode.h"
#include "nodes/UnaryNode.h"

const Node *FloatOperatorSet::forBinary(const BinaryNode *node) const
{
	auto result = new FloatBinaryNode();
	result->ast = node->ast;
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}

const Node *FloatOperatorSet::forUnary(const UnaryNode *node) const
{
	auto result = new FloatUnaryNode();
	result->ast = node->ast;
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}
