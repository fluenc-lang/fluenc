#ifndef UNARYNODE_H
#define UNARYNODE_H

#include "Node.h"

class Node;

class UnaryNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		UnaryNode(const Node *consumer, const std::string &op);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

	private:
		const BaseValue *resolveOp(const BaseValue *value) const;

		const Node *m_consumer;

		std::string m_op;
};

#endif // UNARYNODE_H
