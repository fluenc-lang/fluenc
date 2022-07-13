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

		std::vector<DzResult<BaseValue> > accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue> > accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		const ReferenceValue *m_index;
		const Node *m_node;
		const Type *m_iteratorType;
};

#endif // ARRAYCONTINUATIONNODE_H
