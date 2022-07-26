#ifndef LAZYEVALUATIONNODE_H
#define LAZYEVALUATIONNODE_H

#include "Node.h"

class IIteratable;
class LazyValue;

class LazyEvaluationNode : public Node
{
	friend class Emitter;

	public:
		LazyEvaluationNode(const Node *consumer);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::vector<DzResult> digestDepth(const EntryPoint &entryPoint, Stack values) const;

		const Node *m_consumer;
};

#endif // LAZYEVALUATIONNODE_H
