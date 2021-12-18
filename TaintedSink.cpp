#include "TaintedSink.h"

#include "values/TaintedValue.h"

TaintedSink::TaintedSink(DzValue *subject)
	: m_subject(subject)
{
}

std::vector<DzResult> TaintedSink::build(const EntryPoint &entryPoint, Stack values) const
{
	std::vector<DzResult> taintedResults;

	for (auto &[resultEntryPoint, resultValues] : m_subject->build(entryPoint, values))
	{
		Stack taintedValues;

		for (auto &value : resultValues)
		{
			auto tainted = new TaintedValue(value);

			taintedValues.push(tainted);
		}

		taintedResults.push_back({ resultEntryPoint, taintedValues });
	}

	return taintedResults;
}
