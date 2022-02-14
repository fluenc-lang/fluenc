#include "ArraySink.h"

#include "values/ArrayValue.h"

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
	auto val = m_firstValue->build(entryPoint, Stack());

	auto lazy = new ArrayValue(entryPoint
		, m_iteratorType
		, m_iterator
		, val
		);

	values.push(lazy);

	return m_consumer->build(entryPoint, values);
}
