#include "ArraySinkNode.h"

#include "values/LazyValue.h"
#include "values/ArrayValueGenerator.h"

#include "types/IteratorType.h"

ArraySinkNode::ArraySinkNode(size_t size
	, const Node *consumer
	, const Node *firstValue
	)
	: m_size(size)
	, m_consumer(consumer)
	, m_firstValue(firstValue)
{
}

std::vector<DzResult> ArraySinkNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto arrayContents = m_firstValue->build(entryPoint, Stack());

	auto generator = new ArrayValueGenerator(arrayContents, id(), m_size);
	auto lazy = new LazyValue(generator);

	values.push(lazy);

	std::vector<DzResult> results;

	for (auto &[arrayEntryPoint, _] : arrayContents)
	{
		for (auto &result : m_consumer->build(arrayEntryPoint, values))
		{
			results.push_back(result);
		}
	}

	return results;
}
