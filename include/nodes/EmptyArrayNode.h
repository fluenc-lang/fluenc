#ifndef EMPTYARRAYNODE_H
#define EMPTYARRAYNODE_H

#include "Node.h"

class EmptyArrayNode : public Node
{
	public:
		EmptyArrayNode(Node *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		Node *m_consumer;
};

#endif // EMPTYARRAYNODE_H
