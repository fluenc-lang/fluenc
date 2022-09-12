#ifndef LOCALNODE_H
#define LOCALNODE_H

#include "Node.h"

class LocalNode : public VisitableNode<LocalNode>
{
	friend class Emitter;
	friend class Analyzer;
	friend class NodeLocator;

	public:
		LocalNode(const Node *consumer, const std::string &name);

	private:
		const Node *m_consumer;

		std::string m_name;
};


#endif // LOCALNODE_H
