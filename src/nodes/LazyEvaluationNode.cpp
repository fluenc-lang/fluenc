#include "IteratorStorage.h"

#include "nodes/LazyEvaluationNode.h"

#include "values/IteratorValue.h"
#include "values/ArrayValue.h"
#include "values/TupleValue.h"
#include "values/LazyValue.h"
#include "values/StringValue.h"
#include "values/ForwardedValue.h"
#include "values/ExpandedValue.h"
#include "values/ExpandableValue.h"

#include "types/IteratorType.h"
#include "types/PlaceholderType.h"

LazyEvaluationNode::LazyEvaluationNode(const Node *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> LazyEvaluationNode::digestDepth(const EntryPoint &entryPoint, Stack values) const
{
	for (auto i = 0u; i < values.size(); i++)
	{
		auto value = values.pop();

		if (auto forwarded = dynamic_cast<const ForwardedValue *>(value))
		{
			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : digestDepth(entryPoint, values))
			{
				std::vector<const BaseValue *> forwardedValues;

				for (auto resultValue : resultValues)
				{
					forwardedValues.push_back(resultValue);
				}

				auto subject = forwarded->subject();

				forwardedValues.push_back(subject);

				results.push_back({ resultEntryPoint, forwardedValues });
			}

			return results;
		}

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

		if (auto string = dynamic_cast<const StringValue *>(value))
		{
			auto iterator =	string->iterator();

			auto forwardedValues = values;

			forwardedValues.push(iterator);

			return digestDepth(entryPoint, forwardedValues);
		}

		if (auto tuple = dynamic_cast<const TupleValue *>(value))
		{
			for (auto &element : tuple->values())
			{
				if (auto expanded = dynamic_cast<const ExpandedValue *>(element))
				{
					auto node = expanded->node();

					std::vector<DzResult> results;

					for (auto &[resultEntryPoint, resultValues] : node->build(entryPoint, values))
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
			}

			std::vector<DzResult> results;

			auto digestedResults = digestDepth(entryPoint
				, tuple->values()
				);

			for (auto &[digestedEntryPoint, digestedValues] : digestedResults)
			{
				auto digestedTuple = new TupleValue({ digestedValues.begin(), digestedValues.end() });

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
