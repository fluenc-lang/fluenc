#ifndef RETURNNODE_H
#define RETURNNODE_H

#include "Node.h"

class Type;

class ReturnNode : public VisitableNode<ReturnNode>
{
	friend class Emitter;
	friend class Analyzer;
	friend class NodeLocator;

	public:
		ReturnNode(const Type *iteratorType
			, const Node *consumer
			, const Node *chained
			);

	private:
		const Type *m_iteratorType;

		const Node *m_consumer;
		const Node *m_chained;
};

#endif // RETURNNODE_H
