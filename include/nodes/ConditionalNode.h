#ifndef CONDITIONALNODE_H
#define CONDITIONALNODE_H

#include "Node.h"

class ConditionalNode : public Node
{
	friend class Emitter;

	public:
		ConditionalNode(const Node *ifFalse, const Node *ifTrue);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_ifTrue;
		const Node *m_ifFalse;
};

#endif // CONDITIONALNODE_H
