#include "ArraySink.h"

#include "values/LazyValue.h"
#include "values/ArrayValueGenerator.h"

ArraySink::ArraySink(const Type *iteratorType
	, const DzValue *consumer
	, const DzValue *iterator
	, const DzValue *firstValue
	)
	: m_iteratorType(iteratorType)
	, m_consumer(consumer)
	, m_iterator(iterator)
	, m_firstValue(firstValue)
{
}

std::vector<DzResult> ArraySink::build(const EntryPoint &entryPoint, Stack values) const
{
	auto arrayContents = m_firstValue->build(entryPoint, Stack());

	auto generator = new ArrayValueGenerator(m_iterator, arrayContents);
	auto lazy = new LazyValue(id(), generator);

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
