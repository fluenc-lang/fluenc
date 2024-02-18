#include "Pod.h"
#include "ast.hpp"
#include "Emitter.h"

Pod::Pod(fragment* subject, const Stack &outputValues)
	: m_subject(subject)
	, m_outputValues(outputValues)
{
}

std::vector<DzResult> Pod::build(const Emitter &visitor, DefaultVisitorContext context) const
{
	std::vector<DzResult> results;

	for (auto &[callEntryPoint, callValues] : m_subject->build(visitor, context))
	{
		auto forwardedValues = m_outputValues;

		for (auto &value : callValues)
		{
			forwardedValues.push(value);
		}

		results.push_back({ callEntryPoint, forwardedValues });
	}

	return results;
}
