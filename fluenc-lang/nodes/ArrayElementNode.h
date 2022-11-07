#ifndef ARRAYELEMENTNODE_H
#define ARRAYELEMENTNODE_H

#include <numeric>

#include "Node.h"

class Type;

class ArrayElementNode : public VisitableNode<ArrayElementNode>
{
	friend class Emitter;

	public:
		ArrayElementNode(const std::shared_ptr<peg::Ast> &ast
			, const Type *arrayType
			, const Node *node
			, const Node *next
			);

	private:
		const std::shared_ptr<peg::Ast> m_ast;

		const Type *m_arrayType;

		const Node *m_node;
		const Node *m_next;
};

#endif // ARRAYELEMENTNODE_H
