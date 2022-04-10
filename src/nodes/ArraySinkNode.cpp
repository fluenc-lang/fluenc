#include "ArraySinkNode.h"

#include "values/LazyValue.h"
#include "values/ArrayValueGenerator.h"

ArraySinkNode::ArraySinkNode(const Type *iteratorType
	, const Node *consumer
	, const Node *iterator
	, const Node *firstValue
	)
	: m_iteratorType(iteratorType)
	, m_consumer(consumer)
	, m_iterator(iterator)
	, m_firstValue(firstValue)
{
}

std::vector<DzResult> ArraySinkNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto arrayContents = m_firstValue->build(entryPoint, Stack());

	auto generator = new ArrayValueGenerator(id(), m_iterator, arrayContents);
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
