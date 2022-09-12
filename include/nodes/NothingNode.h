#ifndef NOTHINGNODE_H
#define NOTHINGNODE_H

#include "Node.h"

class NothingNode : public VisitableNode<NothingNode>
{
	friend class Emitter;
	friend class Analyzer;
	friend class NodeLocator;

	public:
		NothingNode(const Node *consumer);

	private:
		const Node *m_consumer;
};

#endif // NOTHINGNODE_H
