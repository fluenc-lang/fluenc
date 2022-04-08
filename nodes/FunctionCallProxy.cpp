#include "FunctionCallProxy.h"
#include "AllIterator.h"

#include "nodes/CallableNode.h"

#include "values/LazyValue.h"
#include "values/IteratorValueGenerator.h"

#include "types/IteratorType.h"

#include "iterators/ExtremitiesIterator.h"

// TODO Could this be moved into StackSegment instead?
FunctionCallProxy::FunctionCallProxy(const std::string name, const Node *consumer, const Node *candidate)
	: m_name(name)
	, m_consumer(consumer)
	, m_subject(candidate)
{
}

std::vector<DzResult> FunctionCallProxy::regularCall(const EntryPoint &entryPoint, Stack values) const
{
	auto functions = entryPoint.functions();

	for (auto [i, end] = functions.equal_range(m_name); i != end; i++)
	{
		auto function = i->second;

		// Naive. Really naive.
		if (function->attribute() == FunctionAttribute::Iterator)
		{
			auto generator = new IteratorValueGenerator(m_subject, entryPoint);
			auto lazy = new LazyValue(generator);

			values.push(lazy);

			return m_consumer->build(entryPoint, values);
		}
	}

	std::vector<DzResult> results;

	for (auto &[subjectEntryPoint, subjectValues] : m_subject->build(entryPoint, values))
	{
		for (auto &consumerResult : m_consumer->build(subjectEntryPoint, subjectValues))
		{
			results.push_back(consumerResult);
		}
	}

	return results;
}

std::vector<DzResult> FunctionCallProxy::build(const EntryPoint &entryPoint, Stack values) const
{
	auto tailCallCandidate = entryPoint
		.byName(m_name);

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

	// Tail call

	return m_subject->build(entryPoint, values);
}
