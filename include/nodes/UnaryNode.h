#ifndef UNARYNODE_H
#define UNARYNODE_H

#include "Node.h"

class Node;

class UnaryNode : public Node
{
	public:
		UnaryNode(const Node *consumer, const std::string &op);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const BaseValue *resolveOp(const BaseValue *value) const;

		const Node *m_consumer;

		std::string m_op;
};

#endif // UNARYNODE_H
