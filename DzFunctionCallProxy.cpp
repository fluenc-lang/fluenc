#include "DzFunctionCallProxy.h"
#include "AllIterator.h"
#include "DzCallable.h"

#include "values/LazyValue.h"
#include "values/IteratorValueGenerator.h"

#include "types/IteratorType.h"

// TODO Could this be moved into StackSegment instead?
DzFunctionCallProxy::DzFunctionCallProxy(const std::string name, const DzValue *consumer, const DzValue *candidate)
	: m_name(name)
	, m_consumer(consumer)
	, m_subject(candidate)
{
}

std::vector<DzResult> DzFunctionCallProxy::regularCall(const EntryPoint &entryPoint, Stack values) const
{
	auto functions = entryPoint.functions();

	for (auto [i, end] = functions.equal_range(m_name); i != end; i++)
	{
		auto function = i->second;

		// Naive. Really naive.
		if (function->attribute() == FunctionAttribute::Iterator)
		{
			auto generator = new IteratorValueGenerator(m_subject, entryPoint);
			auto lazy = new LazyValue(LazyValue::NoCache, generator);

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

std::vector<DzResult> DzFunctionCallProxy::build(const EntryPoint &entryPoint, Stack values) const
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

	auto result = true;

	std::transform(targetValues.begin(), targetValues.end(), inputValues.begin(), all_true(result), [=](auto storage, auto value)
	{
		auto storageType = storage->type();
		auto valueType = value->type();

		return valueType->equals(storageType, entryPoint);
	});

	if (!result)
	{
		return regularCall(entryPoint, values);
	}

	// Tail call

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
