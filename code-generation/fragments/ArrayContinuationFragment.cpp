#include "ArrayContinuationFragment.h"
#include "Emitter.h"

ArrayContinuationNode::ArrayContinuationNode(const reference_value *index, const fragment *node, const Type *iteratorType)
	: m_index(index)
	, m_node(node)
	, m_iteratorType(iteratorType)
{
}

std::vector<DzResult> ArrayContinuationNode::build(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visit(this, context);
}
