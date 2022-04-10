#include "nodes/ExpansionNode.h"

#include "values/ExpandableValue.h"
#include "values/DependentValue.h"

ExpansionNode::ExpansionNode(Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> ExpansionNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto block = entryPoint.block();

	auto iterator = values.require<ExpandableValue>();

	auto continuation = iterator->chain();
	auto provider = iterator->provider();
	auto continuationEntryPoint = provider->withBlock(block);

	auto result = continuation->build(continuationEntryPoint, Stack());

	for (auto &[targetEntryPoint, _] : result)
	{
		auto value = new DependentValue(iterator->type()
			, new EntryPoint(targetEntryPoint)
			);

		values.push(value);

		return m_consumer->build(entryPoint, values);
	}

	throw new std::exception();
}
