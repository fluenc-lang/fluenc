#ifndef NOTHINGNODE_H
#define NOTHINGNODE_H

#include "Node.h"

class NothingNode : public Node
{
	friend class Emitter;

	public:
		NothingNode(const Node *consumer);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_consumer;
};

#endif // NOTHINGNODE_H
