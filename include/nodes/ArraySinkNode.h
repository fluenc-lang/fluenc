#ifndef ARRAYSINKNODE_H
#define ARRAYSINKNODE_H

#include "Node.h"

class Type;

class ArraySinkNode : public VisitableNode<ArraySinkNode>
{
	friend class Emitter;

	public:
		ArraySinkNode(size_t size
			, const std::shared_ptr<peg::Ast> &ast
			, const Node *consumer
			, const Node *firstValue
			);

	private:
		size_t m_size;

		const std::shared_ptr<peg::Ast> m_ast;

		const Node *m_consumer;
		const Node *m_firstValue;
};

#endif // ARRAYSINKNODE_H
