#include "PreEvaluationNode.h"

PreEvaluationNode::PreEvaluationNode(const Visitable<DefaultNodeVisitor> *consumer)
	: m_consumer(consumer)
{
}
