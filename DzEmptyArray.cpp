#include "DzEmptyArray.h"

DzEmptyArray::DzEmptyArray(DzValue *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> DzEmptyArray::build(const EntryPoint &entryPoint, Stack values) const
{
	auto ep = entryPoint
		.pushTag(nextTag());

	return m_consumer->build(ep, values);
}
