#ifndef CONDITIONALNODE_H
#define CONDITIONALNODE_H

#include "Node.h"

class ConditionalNode : public VisitableNode<ConditionalNode>
{
	friend class Emitter;

	public:
		ConditionalNode(const Node *ifFalse, const Node *ifTrue);

	private:
		const Node *m_ifTrue;
		const Node *m_ifFalse;
};

#endif // CONDITIONALNODE_H
