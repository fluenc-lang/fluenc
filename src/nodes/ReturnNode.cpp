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

std::vector<DzResult<BaseValue>> ReturnNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitReturn(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> ReturnNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitReturn(this, entryPoint, values);
}
