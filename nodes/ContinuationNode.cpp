#include <numeric>

#include "ContinuationNode.h"
#include "IndexIterator.h"
#include "ValueHelper.h"

#include "values/ExpandedValue.h"
#include "values/TupleValue.h"

template<typename Container>
const EntryPoint findTailCallTarget(const EntryPoint &candidate, Container container)
{
	return std::accumulate(container.rbegin(), container.rend(), candidate, [&](auto target, auto value)
	{
		if (auto tupleValue = dynamic_cast<const TupleValue *>(value))
		{
			return findTailCallTarget(target
				, tupleValue->values()
				);
		}

		if (auto expandedValue = dynamic_cast<const ExpandedValue *>(value))
		{
			auto provider = expandedValue->provider();

			if (provider->depth() < target.depth())
			{
				return *provider;
			}
		}

		return target;
	});
}

std::vector<DzResult> ContinuationNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto numberOfArguments = values.size();

	auto inputValues = values;
	auto tailCallValues = entryPoint.values();

	auto tailCallCandidate = std::accumulate(index_iterator(0u), index_iterator(numberOfArguments), entryPoint, [&](auto target, size_t)
	{
		return ValueHelper::transferValue(target
			, inputValues.pop()
			, tailCallValues.pop()
			);
	});

	return {{ findTailCallTarget(tailCallCandidate, values), Stack() }};
}
