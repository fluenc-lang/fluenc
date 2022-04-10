#ifndef EXPANSIONNODE_H
#define EXPANSIONNODE_H

#include "Node.h"

class ExpansionNode : public Node
{
	public:
		ExpansionNode(Node *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_consumer;
};

#endif // EXPANSIONNODE_H
