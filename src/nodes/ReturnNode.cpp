#include "nodes/ReturnNode.h"

ReturnNode::ReturnNode(const Type *iteratorType
	, const Node *consumer
	, const Node *chained
	)
	: m_iteratorType(iteratorType)
	, m_consumer(consumer)
	, m_chained(chained)
{
}

std::vector<DzResult> ReturnNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitReturn(this, context);
}

std::vector<DzResult> ReturnNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitReturn(this, context);
}
