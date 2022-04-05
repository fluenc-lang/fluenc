#ifndef ARRAYSINKNODE_H
#define ARRAYSINKNODE_H

#include "Node.h"

class Type;

class ArraySinkNode : public Node
{
	public:
		ArraySinkNode(const Type *iteratorType
			, const Node *consumer
			, const Node *iterator
			, const Node *firstValue
			);

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override;

	private:
		const Type *m_iteratorType;

		const Node *m_consumer;
		const Node *m_iterator;
		const Node *m_firstValue;
};

#endif // ARRAYSINKNODE_H
