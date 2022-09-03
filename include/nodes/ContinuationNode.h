#ifndef CONTINUATIONNODE_H
#define CONTINUATIONNODE_H

#include "Node.h"

class ContinuationNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		ContinuationNode(const Node *node, const Type *iteratorType);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

	private:
		const Node *m_node;
		const Type *m_iteratorType;
};

#endif // CONTINUATIONNODE_H
