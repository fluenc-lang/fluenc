#include "DzEmptyArray.h"

DzEmptyArray::DzEmptyArray(DzValue *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> DzEmptyArray::build(const EntryPoint &entryPoint, Stack values) const
{
	auto ep = entryPoint
		.withIteratorDepth(entryPoint.iteratorDepth() + 1);

	return m_consumer->build(ep, values);
}
