#ifndef EXPANSIONNODE_H
#define EXPANSIONNODE_H

#include "Node.h"
#include "TokenInfo.h"

class ExpansionNode : public VisitableNode<ExpansionNode>
{
	friend class Emitter;
	friend class Analyzer;
	friend class NodeLocator;

	public:
		ExpansionNode(const Node *consumer, const std::shared_ptr<peg::Ast> &ast);

	private:
		const Node *m_consumer;

		std::shared_ptr<peg::Ast> m_ast;
};

#endif // EXPANSIONNODE_H
