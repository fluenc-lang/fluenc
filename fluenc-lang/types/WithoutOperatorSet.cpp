#include "WithoutOperatorSet.h"

#include "nodes/BinaryNode.h"
#include "nodes/UnaryNode.h"

const Node *WithoutOperatorSet::forBinary(const BinaryNode *node) const
{
	auto result = new WithoutBinaryNode();
	result->ast = node->ast;
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}

const Node *WithoutOperatorSet::forUnary(const UnaryNode *node) const
{
	auto result = new WithoutUnaryNode();
	result->ast = node->ast;
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}
