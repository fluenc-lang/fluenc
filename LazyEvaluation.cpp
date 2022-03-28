#include "LazyEvaluation.h"

#include "values/IteratorValue.h"
#include "values/ArrayValue.h"
#include "values/TupleValue.h"
#include "values/LazyValue.h"

LazyEvaluation::LazyEvaluation(const DzValue *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> LazyEvaluation::digestDepth(const EntryPoint &entryPoint, Stack values) const
{
	for (auto i = 0u; i < values.size(); i++)
	{
		auto value = values.pop();

		if (auto lazy = dynamic_cast<const LazyValue *>(value))
		{
			auto iteratable = resolveIteratable(lazy, entryPoint);

			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : iteratable->build(entryPoint))
			{
				auto forwardedValues = values;

				for (auto &resultValue : resultValues)
				{
					forwardedValues.push(resultValue);
				}

				for (auto &result : digestDepth(resultEntryPoint, forwardedValues))
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

std::vector<DzResult> LazyEvaluation::build(const EntryPoint &entryPoint, Stack values) const
{
	std::vector<DzResult> results;

	for (auto &[resultEntryPoint, resultValues] : digestDepth(entryPoint, values))
	{
		for (auto &result : m_consumer->build(resultEntryPoint, resultValues))
		{
			results.push_back(result);
		}
	}

	return results;
}

IIteratable *LazyEvaluation::resolveIteratable(const LazyValue *lazy, const EntryPoint &entryPoint) const
{
	auto existing = m_generated.find(lazy->id());

	if (existing == m_generated.end())
	{
		auto iteratable = lazy->generate(entryPoint);

		if (lazy->id() != LazyValue::NoCache)
		{
			m_generated.insert({ lazy->id(), iteratable });
		}

		return iteratable;
	}

	return existing->second;
}
