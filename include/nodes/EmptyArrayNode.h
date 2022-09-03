#ifndef EMPTYARRAYNODE_H
#define EMPTYARRAYNODE_H

#include "Node.h"

class EmptyArrayNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		EmptyArrayNode(const Node *consumer);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

	private:
		const Node *m_consumer;
};

#endif // EMPTYARRAYNODE_H
