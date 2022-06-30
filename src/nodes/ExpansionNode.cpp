#include "nodes/ExpansionNode.h"
#include "nodes/ContinuationNode.h"

#include "values/ExpandableValue.h"
#include "values/ExpandedValue.h"
#include "values/TupleValue.h"
#include "values/PlaceholderValue.h"
#include "values/BaseValue.h"

ExpansionNode::ExpansionNode(const Node *consumer, const std::shared_ptr<peg::Ast> &ast)
	: m_consumer(consumer)
	, m_ast(ast)
{
}

std::vector<DzResult> ExpansionNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto block = entryPoint.block();

	auto expandable = values.require<ExpandableValue>(m_ast);

	auto continuation = expandable->chain();
	auto provider = expandable->provider();
	auto continuationEntryPoint = provider->withBlock(block);

	auto result = continuation->build(continuationEntryPoint, Stack());

	for (auto &[targetEntryPoint, continuationValues] : result)
	{
		auto value = continuationValues
			.require<ExpandedValue>(nullptr);

		auto tuple = new TupleValue({ value, PlaceholderValue::instance() });

		values.push(tuple);

		auto consumerEntryPoint = entryPoint
			.withBlock(targetEntryPoint.block());

		return m_consumer->build(consumerEntryPoint, values);
	}

	throw new std::exception();
}
