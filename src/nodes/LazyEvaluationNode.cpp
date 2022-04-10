#include "IteratorStorage.h"
#include "nodes/LazyEvaluationNode.h"

#include "values/IteratorValue.h"
#include "values/ArrayValue.h"
#include "values/TupleValue.h"
#include "values/LazyValue.h"

LazyEvaluationNode::LazyEvaluationNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> LazyEvaluationNode::digestDepth(const EntryPoint &entryPoint, Stack values) const
{
	for (auto i = 0u; i < values.size(); i++)
	{
		auto value = values.pop();

		if (auto lazy = dynamic_cast<const LazyValue *>(value))
		{
			auto iteratable = lazy->generate(entryPoint);

			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : iteratable->build(entryPoint))
			{
				auto forwardedValues = values;

				for (auto &resultValue : resultValues)
				{
					forwardedValues.push(resultValue);
				}

				auto forwardedEntryPoint = resultEntryPoint
					.withIteratorStorage(entryPoint.iteratorStorage());

				for (auto &result : digestDepth(forwardedEntryPoint, forwardedValues))
				{
					results.push_back(result);
				}
			}

			return results;
		}

		if (auto tuple = dynamic_cast<const TupleValue *>(value))
		{
			std::vector<DzResult> results;

			auto digestedResults = digestDepth(entryPoint
				, tuple->values()
				);

			for (auto &[digestedEntryPoint, digestedValues] : digestedResults)
			{
				auto digestedTuple = new TupleValue(tuple->iteratorType()
					, { digestedValues.begin(), digestedValues.end() }
					);

				for (auto &[resultEntryPoint, resultValues] : digestDepth(digestedEntryPoint, values))
				{
					std::vector<const BaseValue *> forwardedValues;

					for (auto resultValue : resultValues)
					{
						forwardedValues.push_back(resultValue);
					}

					forwardedValues.push_back(digestedTuple);

					results.push_back({ resultEntryPoint, forwardedValues });
				}
			}

			return results;
		}

		std::vector<DzResult> results;

		for (auto &[resultEntryPoint, resultValues] : digestDepth(entryPoint, values))
		{
			std::vector<const BaseValue *> forwardedValues;

			for (auto resultValue : resultValues)
			{
				forwardedValues.push_back(resultValue);
			}

			forwardedValues.push_back(value);

			results.push_back({ resultEntryPoint, forwardedValues });
		}

		return results;
	}

	return {{ entryPoint, values }};
}

EntryPoint tryForkEntryPoint(const EntryPoint &entryPoint)
{
	if (entryPoint.iteratorStorage())
	{
		return entryPoint;
	}

	return entryPoint
		.withIteratorStorage(new IteratorStorage());
}

std::vector<DzResult> LazyEvaluationNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto forkedEntryPoint = tryForkEntryPoint(entryPoint);

	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : digestDepth(forkedEntryPoint, values))
	{
		auto consumerEntryPoint = resultEntryPoint
			.withIteratorStorage(entryPoint.iteratorStorage());

		for (auto &result : m_consumer->build(consumerEntryPoint, resultValues))
		{
			results.push_back(result);
		}
	}

	return results;
}
