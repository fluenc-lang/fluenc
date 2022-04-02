#include "LazySink.h"

#include "values/LazyValue.h"
#include "values/IteratorValueGenerator.h"

LazySink::LazySink(const Type *iteratorType
	, const DzValue *consumer
	, const DzValue *subject
	)
	: m_iteratorType(iteratorType)
	, m_consumer(consumer)
	, m_subject(subject)
{
}

std::vector<DzResult> LazySink::build(const EntryPoint &entryPoint, Stack values) const
{
	auto generator = new IteratorValueGenerator(m_subject, entryPoint);
	auto lazy = new LazyValue(generator);

	values.push(lazy);

	return m_consumer->build(entryPoint, values);
}
