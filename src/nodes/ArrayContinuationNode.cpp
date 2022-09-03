#include "nodes/ArrayContinuationNode.h"

ArrayContinuationNode::ArrayContinuationNode(const ReferenceValue *index, const Node *node, const Type *iteratorType)
	: m_index(index)
	, m_node(node)
	, m_iteratorType(iteratorType)
{
}

std::vector<DzResult> ArrayContinuationNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitArrayContinuation(this, context);
}

std::vector<DzResult > ArrayContinuationNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitArrayContinuation(this, context);
}
