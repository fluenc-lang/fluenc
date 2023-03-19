#ifndef UNARYNODE_H
#define UNARYNODE_H

#include "Node.h"

class Node;

template<typename TSelf>
struct UnaryNodeBase : public VisitableNode<TSelf>
{
	std::shared_ptr<peg::Ast> ast;

	const Node *consumer;

	std::string op;
};

struct IntegerUnaryNode : public UnaryNodeBase<IntegerUnaryNode>
{
};

struct BooleanUnaryNode : UnaryNodeBase<BooleanUnaryNode>
{
};

struct FloatUnaryNode : public UnaryNodeBase<FloatUnaryNode>
{
};

struct StringUnaryNode : public UnaryNodeBase<StringUnaryNode>
{
};

struct ArrayUnaryNode : public UnaryNodeBase<ArrayUnaryNode>
{
};

struct WithoutUnaryNode : public UnaryNodeBase<WithoutUnaryNode>
{
};

struct UnaryNode : public UnaryNodeBase<UnaryNode>
{
};

#endif // UNARYNODE_H
