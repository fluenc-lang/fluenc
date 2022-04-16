#include "nodes/FunctionCallProxyNode.h"
#include "AllIterator.h"
#include "nodes/JunctionNode.h"

#include "nodes/CallableNode.h"

#include "values/LazyValue.h"
#include "values/IteratorValueGenerator.h"

#include "types/IteratorType.h"

#include "iterators/ExtremitiesIterator.h"

// TODO Could this be moved into StackSegment instead?
FunctionCallProxyNode::FunctionCallProxyNode(const std::vector<std::string> &names
	, const Node *consumer
	, const Node *withEvaluation
	, const Node *withoutEvaluation
	)
	: m_names(names)
	, m_consumer(consumer)
	, m_withEvaluation(withEvaluation)
	, m_withoutEvaluation(withoutEvaluation)
{
}

std::vector<DzResult> FunctionCallProxyNode::regularCall(const EntryPoint &entryPoint, Stack values) const
{
	auto functions = entryPoint.functions();

	for (auto &name : m_names)
	{
		for (auto [i, end] = functions.equal_range(name); i != end; i++)
		{
			auto function = i->second;

			// Naive. Really naive.
			if (function->attribute() == FunctionAttribute::Iterator)
			{
				auto generator = new IteratorValueGenerator(new IteratorType(), m_withEvaluation, entryPoint);
				auto lazy = new LazyValue(generator);

				values.push(lazy);

				return m_consumer->build(entryPoint, values);
			}

			if (function->attribute() == FunctionAttribute::Import)
			{
				std::vector<DzResult> results;

				auto junction = new JunctionNode(m_withoutEvaluation);

				for (auto &[subjectEntryPoint, subjectValues] : junction->build(entryPoint, values))
				{
					for (auto &consumerResult : m_consumer->build(subjectEntryPoint, subjectValues))
					{
						results.push_back(consumerResult);
					}
				}

				return results;
			}
		}
	}

	std::vector<DzResult> results;

	auto junction = new JunctionNode(m_withEvaluation);

	for (auto &[subjectEntryPoint, subjectValues] : junction->build(entryPoint, values))
	{
		for (auto &consumerResult : m_consumer->build(subjectEntryPoint, subjectValues))
		{
			results.push_back(consumerResult);
		}
	}

	return results;
}

std::vector<DzResult> FunctionCallProxyNode::build(const EntryPoint &entryPoint, Stack values) const
{
	for (auto &name : m_names)
	{
		auto tailCallCandidate = entryPoint
			.byName(name);

		if (!tailCallCandidate)
		{
			return regularCall(entryPoint, values);
		}

		auto targetValues = tailCallCandidate->values();
		auto inputValues = values;

		if (targetValues.size() != inputValues.size())
		{
			return regularCall(entryPoint, values);
		}

		int8_t min = 0;
		int8_t max = 0;

		std::transform(targetValues.begin(), targetValues.end(), inputValues.begin(), extremities_iterator(min, max), [=](auto storage, auto value)
		{
			auto storageType = storage->type();
			auto valueType = value->type();

			return valueType->compatibility(storageType, entryPoint);
		});

		if (min < 0 || max > 0)
		{
			return regularCall(entryPoint, values);
		}
	}

	// Tail call

	return m_withEvaluation->build(entryPoint, values);
}
