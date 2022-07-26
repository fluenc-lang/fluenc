#include "nodes/LazyEvaluationNode.h"

LazyEvaluationNode::LazyEvaluationNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> LazyEvaluationNode::accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const
{
	return visitor.visitLazyEvaluation(this, entryPoint, values);
}
