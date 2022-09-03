#ifndef CONDITIONALNODE_H
#define CONDITIONALNODE_H

#include "Node.h"

class ConditionalNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		ConditionalNode(const Node *ifFalse, const Node *ifTrue);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

	private:
		const Node *m_ifTrue;
		const Node *m_ifFalse;
};

#endif // CONDITIONALNODE_H
