#ifndef CONDITIONALNODE_H
#define CONDITIONALNODE_H

#include "Node.h"

class ConditionalNode : public Node
{
	public:
		ConditionalNode(Node *ifFalse, Node *ifTrue);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_ifTrue;
		Node *m_ifFalse;
};

#endif // CONDITIONALNODE_H
