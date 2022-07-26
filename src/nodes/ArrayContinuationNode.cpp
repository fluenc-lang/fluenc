#include "nodes/ArrayContinuationNode.h"

ArrayContinuationNode::ArrayContinuationNode(const ReferenceValue *index, const Node *node, const Type *iteratorType)
	: m_index(index)
	, m_node(node)
	, m_iteratorType(iteratorType)
{
}

std::vector<DzResult> ArrayContinuationNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitArrayContinuation(this, entryPoint, values);
}
