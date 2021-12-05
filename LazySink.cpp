#include "LazySink.h"

#include "values/LazyValue.h"

LazySink::LazySink(const DzValue *consumer, const DzValue *subject)
	: m_consumer(consumer)
	, m_subject(subject)
{
}

std::vector<DzResult> LazySink::build(const EntryPoint &entryPoint, Stack values) const
{
	auto lazy = new LazyValue(m_subject, entryPoint);

	values.push(lazy);

	return m_consumer->build(entryPoint, values);
}
