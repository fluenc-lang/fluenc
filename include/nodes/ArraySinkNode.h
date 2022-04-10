#ifndef ARRAYSINKNODE_H
#define ARRAYSINKNODE_H

#include "Node.h"

class Type;

class ArraySinkNode : public Node
{
	public:
		ArraySinkNode(size_t size
			, const Node *consumer
			, const Node *firstValue
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		size_t m_size;

		const Node *m_consumer;
		const Node *m_firstValue;
};

#endif // ARRAYSINKNODE_H
