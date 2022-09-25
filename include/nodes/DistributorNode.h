#ifndef DISTRIBUTORNODE_H
#define DISTRIBUTORNODE_H

#include "Node.h"

class DistributorNode : public VisitableNode<DistributorNode>
{
	friend class Emitter;
	friend class Analyzer;

	public:
		DistributorNode(const Node *subject, const Node *consumer)
			: m_subject(subject)
			, m_consumer(consumer)
		{
		}

	private:
		const Node *m_subject;
		const Node *m_consumer;
};

#endif // DISTRIBUTORNODE_H
