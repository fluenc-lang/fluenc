#include "nodes/ArrayContinuationNode.h"

ArrayContinuationNode::ArrayContinuationNode(const ReferenceValue *index, const Node *node, const Type *iteratorType)
	: m_index(index)
	, m_node(node)
	, m_iteratorType(iteratorType)
{
}

std::vector<DzResult<BaseValue>> ArrayContinuationNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitArrayContinuation(this, entryPoint, values);
}

std::vector<DzResult<BaseValue> > ArrayContinuationNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitArrayContinuation(this, entryPoint, values);
}
