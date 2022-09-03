#include "nodes/LazyEvaluationNode.h"

LazyEvaluationNode::LazyEvaluationNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> LazyEvaluationNode::accept(const Emitter &visitor, DefaultVisitorContext context) const
{
	return visitor.visitLazyEvaluation(this, context);
}

std::vector<DzResult> LazyEvaluationNode::accept(const Analyzer &visitor, DefaultVisitorContext context) const
{
	return visitor.visitLazyEvaluation(this, context);
}
