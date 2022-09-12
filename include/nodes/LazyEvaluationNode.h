#ifndef LAZYEVALUATIONNODE_H
#define LAZYEVALUATIONNODE_H

#include "Node.h"

class IIteratable;
class LazyValue;

class LazyEvaluationNode : public VisitableNode<LazyEvaluationNode>
{
	friend class Emitter;
	friend class Analyzer;
	friend class NodeLocator;

	public:
		LazyEvaluationNode(const Node *consumer);

	private:
		const Node *m_consumer;
};

#endif // LAZYEVALUATIONNODE_H
