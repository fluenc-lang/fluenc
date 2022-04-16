#ifndef EXPANSIONNODE_H
#define EXPANSIONNODE_H

#include "Node.h"

class ExpansionNode : public Node
{
	public:
		ExpansionNode(const Node *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_consumer;
};

#endif // EXPANSIONNODE_H
