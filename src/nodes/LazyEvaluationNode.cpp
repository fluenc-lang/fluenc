#include "nodes/LazyEvaluationNode.h"

LazyEvaluationNode::LazyEvaluationNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult<BaseValue>> LazyEvaluationNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitLazyEvaluation(this, entryPoint, values);
}

std::vector<DzResult<BaseValue>> LazyEvaluationNode::accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const
{
	return visitor.visitLazyEvaluation(this, entryPoint, values);
}
