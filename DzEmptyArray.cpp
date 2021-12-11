#include "DzEmptyArray.h"

#include "values/WithoutValue.h"
#include "values/TaintedValue.h"

DzEmptyArray::DzEmptyArray(DzValue *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> DzEmptyArray::build(const EntryPoint &entryPoint, Stack values) const
{
	auto tainted = new TaintedValue(WithoutValue::instance());

	values.push(tainted);

	return m_consumer->build(entryPoint, values);
}
