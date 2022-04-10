#include "IndexSinkNode.h"

IndexSinkNode::IndexSinkNode(size_t index, const Node *consumer)
	: m_index(index)
	, m_consumer(consumer)
{
}

std::vector<DzResult> IndexSinkNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto value = values.pop();

	auto indexed = new IndexedValue(m_index, value);

	values.push(indexed);

	return m_consumer->build(entryPoint, values);
}
