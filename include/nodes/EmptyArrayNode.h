#ifndef EMPTYARRAYNODE_H
#define EMPTYARRAYNODE_H

#include "Node.h"

class EmptyArrayNode : public VisitableNode<EmptyArrayNode>
{
	friend class Emitter;
	friend class Analyzer;
	friend class NodeLocator;

	public:
		EmptyArrayNode(const Node *consumer);

	private:
		const Node *m_consumer;
};

#endif // EMPTYARRAYNODE_H
