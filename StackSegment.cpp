#include <numeric>

#include "StackSegment.h"

StackSegment::StackSegment(std::vector<DzValue *> values, DzValue *call, DzValue *consumer)
	: m_values(values)
	, m_call(call)
	, m_consumer(consumer)
{
}

int StackSegment::compare(DzValue *other, const EntryPoint &entryPoint) const
{
	UNUSED(other);
	UNUSED(entryPoint);

	return -1;
}

std::vector<DzResult> StackSegment::build(const EntryPoint &entryPoint, Stack values) const
{
	std::vector<DzResult> result;
	std::vector<DzResult> input = {{ entryPoint, Stack() }};

	auto orderedValues = m_values;

	std::sort(begin(orderedValues), end(orderedValues), [=](auto first, auto second)
	{
		return first->compare(second, entryPoint);
	});

	orderedValues.push_back(m_call);

	auto subjectResults = std::accumulate(begin(orderedValues), end(orderedValues), input, [=](auto accumulator, auto value)
	{
		std::vector<DzResult> results;

		for (auto &[accumulatorEntryPoint, accumulatorValues] : accumulator)
		{
			auto result = value->build(accumulatorEntryPoint, accumulatorValues);

			results.insert(end(results), begin(result), end(result));
		}

		return results;
	});

	for (auto &[subjectEntryPoint, subjectValues] : subjectResults)
	{
		auto forwardedValues = values;

		for (auto &value: subjectValues)
		{
			forwardedValues.push(value);
		}

		auto consumerResults = m_consumer->build(subjectEntryPoint, forwardedValues);

		for (auto &consumerResult : consumerResults)
		{
			result.push_back(consumerResult);
		}
	}

	return result;
}
