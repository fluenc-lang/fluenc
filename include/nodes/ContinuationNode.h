#ifndef CONTINUATIONNODE_H
#define CONTINUATIONNODE_H

#include "Node.h"

class ContinuationNode : public Node
{
	friend class Emitter;

	public:
		ContinuationNode(const Node *node, const Type *iteratorType);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_node;
		const Type *m_iteratorType;
};

#endif // CONTINUATIONNODE_H
