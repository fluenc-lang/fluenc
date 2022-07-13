#ifndef INDEXSINKNODE_H
#define INDEXSINKNODE_H

#include "Node.h"

#include "values/IndexedValue.h"

class IndexSinkNode : public Node
{
	friend class Emitter;
	friend class Analyzer;

	public:
		IndexSinkNode(size_t index, const Node *consumer);

		std::vector<DzResult<BaseValue>> accept(const Emitter &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;
		std::vector<DzResult<BaseValue>> accept(const Analyzer &visitor, const EntryPoint &entryPoint, Stack<BaseValue> values) const override;

	private:
		size_t m_index;

		const Node *m_consumer;
};

#endif // INDEXSINKNODE_H
