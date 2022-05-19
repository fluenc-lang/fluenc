#include "nodes/FunctionCallProxyNode.h"
#include "nodes/CallableNode.h"
#include "nodes/JunctionNode.h"

#include "values/LazyValue.h"
#include "values/IteratorValueGenerator.h"

FunctionCallProxyNode::FunctionCallProxyNode(const std::vector<std::string> &names
	, const Node *regularCall
	, const Node *consumer
	)
	: m_names(names)
	, m_regularCall(regularCall)
	, m_consumer(consumer)
{
}

std::vector<DzResult> FunctionCallProxyNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	insertBlock(block, function);

	auto functions = entryPoint.functions();

	for (auto &name : m_names)
	{
		for (auto [i, end] = functions.equal_range(name); i != end; i++)
		{
			auto function = i->second;

			// Naive. Really naive.
			if (function->attribute() == FunctionAttribute::Iterator)
			{
				auto generator = new IteratorValueGenerator(new IteratorType(), m_regularCall, entryPoint);
				auto lazy = new LazyValue(generator);

				auto forwardedValues = values;

				forwardedValues.push(lazy);

				std::vector<DzResult> results;

				for (auto &result : m_consumer->build(entryPoint, forwardedValues))
				{
					results.push_back(result);
				}

				return results;
			}
		}
	}

	std::vector<DzResult> results;

	auto junction = new JunctionNode(m_regularCall);

	for (auto &[resultEntryPoint, resultValues] : junction->build(entryPoint, values))
	{
		for (auto &result : m_consumer->build(resultEntryPoint, resultValues))
		{
			results.push_back(result);
		}
	}

	return results;
}
