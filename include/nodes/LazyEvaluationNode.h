#ifndef LAZYEVALUATIONNODE_H
#define LAZYEVALUATIONNODE_H

#include "Node.h"

class IIteratable;
class LazyValue;

class LazyEvaluationNode : public Node
{
	public:
		LazyEvaluationNode(const Node *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::vector<DzResult> digestDepth(const EntryPoint &entryPoint, Stack values) const;

		const Node *m_consumer;
};

#endif // LAZYEVALUATIONNODE_H
