#include "ArraySink.h"

#include "values/ArrayValue.h"

ArraySink::ArraySink(const Type *iteratorType
	, const DzValue *consumer
	, const DzValue *subject
	)
	: m_iteratorType(iteratorType)
	, m_consumer(consumer)
	, m_subject(subject)
{
}

std::vector<DzResult> ArraySink::build(const EntryPoint &entryPoint, Stack values) const
{
	auto lazy = new ArrayValue(entryPoint
		, m_iteratorType
		, m_subject
		);

	values.push(lazy);

	return m_consumer->build(entryPoint, values);
}
