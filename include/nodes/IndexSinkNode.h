#ifndef INDEXSINKNODE_H
#define INDEXSINKNODE_H

#include "Node.h"

#include "values/IndexedValue.h"

class IndexSinkNode : public VisitableNode<IndexSinkNode>
{
	friend class Emitter;
	friend class Analyzer;
	friend class NodeLocator;

	public:
		IndexSinkNode(size_t index, const Node *consumer);

	private:
		size_t m_index;

		const Node *m_consumer;
};

#endif // INDEXSINKNODE_H
