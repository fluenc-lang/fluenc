#ifndef INDEXSINKNODE_H
#define INDEXSINKNODE_H

#include "Node.h"

#include "values/IndexedValue.h"

class IndexSinkNode : public Node
{
	friend class Emitter;

	public:
		IndexSinkNode(size_t index, const Node *consumer);

		std::vector<DzResult> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack values) const override;

	private:
		size_t m_index;

		const Node *m_consumer;
};

#endif // INDEXSINKNODE_H
