#ifndef LAZYEVALUATIONNODE_H
#define LAZYEVALUATIONNODE_H

#include "Node.h"

class IIteratable;
class LazyValue;

class LazyEvaluationNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		LazyEvaluationNode(const Node *consumer);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Node *m_consumer;
};

#endif // LAZYEVALUATIONNODE_H
