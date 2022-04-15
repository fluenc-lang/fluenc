#include "nodes/ExpansionNode.h"

#include "values/ExpandableValue.h"
#include "values/ExpandedValue.h"
#include "values/TupleValue.h"
#include "values/PlaceholderValue.h"

ExpansionNode::ExpansionNode(const Node *consumer)
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
		auto value = new ExpandedValue(iterator->type()
			, new EntryPoint(targetEntryPoint)
			);

		auto tuple = new TupleValue({ value, PlaceholderValue::instance() });

		values.push(tuple);

		return m_consumer->build(entryPoint, values);
	}

	throw new std::exception();
}
