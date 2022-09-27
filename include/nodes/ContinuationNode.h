#ifndef CONTINUATIONNODE_H
#define CONTINUATIONNODE_H

#include "Node.h"

class ContinuationNode : public VisitableNode<ContinuationNode>
{
	friend class Emitter;

	public:
		ContinuationNode(const Node *node, const Type *iteratorType);

	private:
		const Node *m_node;
		const Type *m_iteratorType;
};

#endif // CONTINUATIONNODE_H
