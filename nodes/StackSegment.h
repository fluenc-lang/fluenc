#ifndef STACKSEGMENT_H
#define STACKSEGMENT_H

#include "Node.h"

class StackSegment : public Node
{
	public:
		StackSegment(std::vector<Node *> values, Node *call, Node *consumer);

		int order(const EntryPoint &entryPoint) const override;

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		std::vector<Node *> m_values;

		Node *m_call;
		Node *m_consumer;
};

#endif // STACKSEGMENT_H
