#ifndef ARRAYCONTINUATIONNODE_H
#define ARRAYCONTINUATIONNODE_H

#include "Node.h"

class ReferenceValue;

class ArrayContinuationNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		ArrayContinuationNode(const ReferenceValue *index, const Node *node, const Type *iteratorType);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

	private:
		const ReferenceValue *m_index;
		const Node *m_node;
		const Type *m_iteratorType;
};

#endif // ARRAYCONTINUATIONNODE_H
