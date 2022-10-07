#ifndef UNARYNODE_H
#define UNARYNODE_H

#include "Node.h"

class Node;

class UnaryNode : public VisitableNode<UnaryNode>
{
	friend class Emitter;

	public:
		UnaryNode(const std::shared_ptr<peg::Ast> &ast, const Node *consumer, const std::string &op);

	private:
		const BaseValue *resolveOp(const BaseValue *value) const;

		const std::shared_ptr<peg::Ast> m_ast;

		const Node *m_consumer;

		std::string m_op;
};

#endif // UNARYNODE_H
