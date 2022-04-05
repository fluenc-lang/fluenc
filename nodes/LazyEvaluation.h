#ifndef LAZYEVALUATION_H
#define LAZYEVALUATION_H

#include "Node.h"

class IIteratable;
class LazyValue;

class LazyEvaluation : public Node
{
	public:
		LazyEvaluation(const Node *consumer);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::vector<DzResult> digestDepth(const EntryPoint &entryPoint, Stack values) const;

		const Node *m_consumer;
};

#endif // LAZYEVALUATION_H
