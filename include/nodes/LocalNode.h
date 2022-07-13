#ifndef LOCALNODE_H
#define LOCALNODE_H

#include "Node.h"

class LocalNode : public Node
{
	friend class Emitter;

	public:
		LocalNode(const Node *consumer, const std::string &name);

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		const Node *m_consumer;

		std::string m_name;
};


#endif // LOCALNODE_H
