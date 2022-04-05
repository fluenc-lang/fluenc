#ifndef ARRAYSINK_H
#define ARRAYSINK_H

#include "Node.h"

class Type;

class ArraySink : public Node
{
	public:
		ArraySink(const Type *iteratorType
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

#endif // ARRAYSINK_H
