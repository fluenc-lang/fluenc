#include "StackSegment.h"

StackSegment::StackSegment(DzValue *subject, DzValue *consumer)
	: m_subject(subject)
	, m_consumer(consumer)
{
}

std::vector<DzResult> StackSegment::build(const EntryPoint &entryPoint, Stack values) const
{
	std::vector<DzResult> result;

	auto subjectResults = m_subject->build(entryPoint, Stack());

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
