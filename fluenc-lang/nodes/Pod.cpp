#include "Pod.h"

Pod::Pod(const Node *subject, const Stack &outputValues)
	: m_subject(subject)
	, m_outputValues(outputValues)
{
}

std::vector<DzResult> Pod::accept(const DefaultNodeVisitor &visitor, DefaultVisitorContext context) const
{
	std::vector<DzResult> results;

	for (auto &[callEntryPoint, callValues] : m_subject->accept(visitor, context))
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
