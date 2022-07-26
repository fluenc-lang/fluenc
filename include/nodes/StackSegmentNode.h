#ifndef STACKSEGMENTNODE_H
#define STACKSEGMENTNODE_H

#include "Node.h"

class StackSegmentNode : public Node
{
	friend class Emitter;

	public:
		StackSegmentNode(std::vector<Node *> values, const Node *call, const Node *consumer);

		int order(const EntryPoint &entryPoint) const override;

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::vector<Node *> m_values;

		const Node *m_call;
		const Node *m_consumer;
};

#endif // STACKSEGMENTNODE_H
