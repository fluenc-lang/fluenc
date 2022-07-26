#ifndef EMPTYARRAYNODE_H
#define EMPTYARRAYNODE_H

#include "Node.h"

class EmptyArrayNode : public Node
{
	friend class Emitter;

	public:
		EmptyArrayNode(const Node *consumer);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_consumer;
};

#endif // EMPTYARRAYNODE_H
