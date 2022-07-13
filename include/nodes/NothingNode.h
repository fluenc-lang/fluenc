#ifndef NOTHINGNODE_H
#define NOTHINGNODE_H

#include "Node.h"

class NothingNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		NothingNode(const Node *consumer);

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		const Node *m_consumer;
};

#endif // NOTHINGNODE_H
