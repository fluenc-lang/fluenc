#ifndef LAZYEVALUATIONNODE_H
#define LAZYEVALUATIONNODE_H

#include "Node.h"

class IIteratable;
class LazyValue;

class LazyEvaluationNode : public VisitableNode<LazyEvaluationNode>
{
	friend class Emitter;

	public:
		LazyEvaluationNode(const Visitable<DefaultNodeVisitor> *consumer);

	private:
		const Visitable<DefaultNodeVisitor> *m_consumer;
};

#endif // LAZYEVALUATIONNODE_H
