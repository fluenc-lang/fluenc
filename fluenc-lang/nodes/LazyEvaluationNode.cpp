#include "nodes/LazyEvaluationNode.h"

LazyEvaluationNode::LazyEvaluationNode(const Visitable<DefaultNodeVisitor> *consumer)
	: m_consumer(consumer)
{
}
