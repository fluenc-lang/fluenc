#include "ArrayOperatorSet.h"

const Node *ArrayOperatorSet::forBinary(const BinaryNode *node) const
{
	auto result = new ArrayBinaryNode();
	result->ast = node->ast;
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}

const Node *ArrayOperatorSet::forUnary(const UnaryNode *node) const
{
	auto result = new ArrayUnaryNode();
	result->ast = node->ast;
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}
