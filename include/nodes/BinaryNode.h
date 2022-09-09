#ifndef BINARYNODE_H
#define BINARYNODE_H

#include "Node.h"

class ScalarValue;

template<typename TSelf>
struct BinaryNodeBase : public VisitableNode<TSelf>
{
	const Node *consumer;

	std::string op;
};

struct IntegerBinaryNode : public BinaryNodeBase<IntegerBinaryNode>
{
};

struct BooleanBinaryNode : BinaryNodeBase<BooleanBinaryNode>
{
};

struct FloatBinaryNode : public BinaryNodeBase<FloatBinaryNode>
{
};

struct BinaryNode : public BinaryNodeBase<BinaryNode>
{
};

#endif // BINARYNODE_H
