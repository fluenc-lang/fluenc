#ifndef NOTHINGNODE_H
#define NOTHINGNODE_H

#include "Node.h"

class NothingNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		NothingNode(const Node *consumer);

		std::vector<DzResult> accept(const Emitter &visitor, DefaultVisitorContext context) const override;
		std::vector<DzResult> accept(const Analyzer &visitor, DefaultVisitorContext context) const override;

	private:
		const Node *m_consumer;
};

#endif // NOTHINGNODE_H
