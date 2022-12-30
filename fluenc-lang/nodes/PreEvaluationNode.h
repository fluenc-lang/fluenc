#ifndef PREEVALUATIONNODE_H
#define PREEVALUATIONNODE_H

#include "Node.h"

class IIteratable;
class LazyValue;

class PreEvaluationNode : public VisitableNode<PreEvaluationNode>
{
	friend class Emitter;

	public:
		PreEvaluationNode(const Visitable<DefaultNodeVisitor> *consumer);

	private:
		const Visitable<DefaultNodeVisitor> *m_consumer;
};

#endif // PREEVALUATIONNODE_H
