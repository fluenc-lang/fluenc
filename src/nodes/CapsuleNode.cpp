#include "nodes/CapsuleNode.h"

CapsuleNode::CapsuleNode(const Stack &values, const Node *subject)
	: m_values(values)
	, m_call(subject)
{
}

std::vector<DzResult> CapsuleNode::build(const EntryPoint &entryPoint, Stack values) const
{
	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : m_call->build(entryPoint, m_values))
	{
		auto forwardedValues = values;

		for (auto &value : resultValues)
		{
			forwardedValues.push(value);
		}

		results.push_back({ resultEntryPoint, forwardedValues });
	}

	return results;
}
