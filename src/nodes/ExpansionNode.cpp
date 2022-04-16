#include "nodes/ExpansionNode.h"

#include "values/ExpandableValue.h"
#include "values/ExpandedValue.h"
#include "values/TupleValue.h"
#include "values/PlaceholderValue.h"
#include "values/BaseValue.h"

ExpansionNode::ExpansionNode(antlr4::ParserRuleContext *context, const Node *consumer)
	: m_token(TokenInfo::fromContext(context))
	, m_consumer(consumer)
{
}

std::vector<DzResult> ExpansionNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto block = entryPoint.block();

	auto expandable = values.require<ExpandableValue>(m_token);

	auto continuation = expandable->chain();
	auto provider = expandable->provider();
	auto continuationEntryPoint = provider->withBlock(block);

	auto result = continuation->build(continuationEntryPoint, Stack());

	for (auto &[targetEntryPoint, _] : result)
	{
		auto value = new ExpandedValue(expandable->type()
			, new EntryPoint(targetEntryPoint)
			);

		auto tuple = new TupleValue({ value, PlaceholderValue::instance() });

		values.push(tuple);

		return m_consumer->build(entryPoint, values);
	}

	throw new std::exception();
}
