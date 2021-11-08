#include "DzExpansion.h"

#include "values/DependentValue.h"

DzExpansion::DzExpansion(DzValue *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> DzExpansion::build(const EntryPoint &entryPoint, Stack values) const
{
	auto block = entryPoint.block();

	auto iterator = values.require<DependentValue>();

	auto continuation = iterator->chain();
	auto provider = iterator->provider();
	auto continuationEntryPoint = provider->withBlock(block);

	continuation->build(continuationEntryPoint, Stack());

	values.push(iterator);

	return m_consumer->build(entryPoint, values);
}
