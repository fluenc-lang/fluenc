#ifndef BINARYNODE_H
#define BINARYNODE_H

#include "Node.h"

class ScalarValue;

class BinaryNodeBase
{
	public:
		BinaryNodeBase(const Node *consumer, const std::string &op)
			: m_consumer(consumer)
			, m_op(op)
		{
		}

		const Node * m_consumer;

		std::string m_op;
};

class IntegerBinaryNode
	: public VisitableNode<IntegerBinaryNode>
	, public BinaryNodeBase
{
	public:
		IntegerBinaryNode(const BinaryNodeBase *other)
			: BinaryNodeBase(other->m_consumer, other->m_op)
		{
		}
};

class BooleanBinaryNode
	: public VisitableNode<BooleanBinaryNode>
	, public BinaryNodeBase
{
	public:
		BooleanBinaryNode(const BinaryNodeBase *other)
			: BinaryNodeBase(other->m_consumer, other->m_op)
		{
		}
};

class FloatBinaryNode
	: public VisitableNode<FloatBinaryNode>
	, public BinaryNodeBase
{
	public:
		FloatBinaryNode(const BinaryNodeBase *other)
			: BinaryNodeBase(other->m_consumer, other->m_op)
		{
		}
};

class BinaryNode : public VisitableNode<BinaryNode>, public BinaryNodeBase
{
	public:
		BinaryNode(const Node *consumer, const std::string &op);
};

#endif // BINARYNODE_H
