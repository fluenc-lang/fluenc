#ifndef ARRAYCONTINUATIONNODE_H
#define ARRAYCONTINUATIONNODE_H

#include "Node.h"

class ReferenceValue;

class ArrayContinuationNode : public VisitableNode<ArrayContinuationNode>
{
	friend class Emitter;

	public:
		ArrayContinuationNode(const ReferenceValue *index, const Node *node, const Type *iteratorType);

	private:
		const ReferenceValue *m_index;
		const Node *m_node;
		const Type *m_iteratorType;
};

#endif // ARRAYCONTINUATIONNODE_H
