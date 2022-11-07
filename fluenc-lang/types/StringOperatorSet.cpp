#include "types/StringOperatorSet.h"

#include "nodes/BinaryNode.h"
#include "nodes/UnaryNode.h"

const Node *StringOperatorSet::forBinary(const BinaryNode *node) const
{
	auto result = new StringBinaryNode();
	result->ast = node->ast;
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}

const Node *StringOperatorSet::forUnary(const UnaryNode *node) const
{
	auto result = new StringUnaryNode();
	result->ast = node->ast;
	result->consumer = node->consumer;
	result->op = node->op;

	return result;
}
