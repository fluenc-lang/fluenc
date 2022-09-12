#ifndef ARRAYSINKNODE_H
#define ARRAYSINKNODE_H

#include "Node.h"

class Type;

class ArraySinkNode : public VisitableNode<ArraySinkNode>
{
	friend class Emitter;
	friend class Analyzer;
	friend class NodeLocator;

	public:
		ArraySinkNode(size_t size
			, const Node *consumer
			, const Node *firstValue
			);

	private:
		size_t m_size;

		const Node *m_consumer;
		const Node *m_firstValue;
};

#endif // ARRAYSINKNODE_H
