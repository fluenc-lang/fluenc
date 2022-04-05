#include "EmptyArray.h"

#include "values/WithoutValue.h"
#include "values/TaintedValue.h"

EmptyArray::EmptyArray(Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> EmptyArray::build(const EntryPoint &entryPoint, Stack values) const
{
	auto tainted = new TaintedValue(WithoutValue::instance());

	values.push(tainted);

	return m_consumer->build(entryPoint, values);
}
