#include "LazyEvaluation.h"

#include "values/LazyValue.h"
#include "values/ArrayValue.h"
#include "values/TupleValue.h"

LazyEvaluation::LazyEvaluation(const DzValue *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> digest(const EntryPoint &entryPoint, Stack values)
{
	for (auto i = 0u; i < values.size(); i++)
	{
		auto value = values.pop();

		if (auto lazy = dynamic_cast<const LazyValue *>(value))
		{
			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : lazy->build(entryPoint.block(), Stack()))
			{
				auto forwardedValues = values;

				for (auto &resultValue : resultValues)
				{
					forwardedValues.push(resultValue);
				}

				for (auto &result : digest(resultEntryPoint, forwardedValues))
				{
					results.push_back(result);
				}
			}

			return results;
		}

		if (auto array = dynamic_cast<const ArrayValue *>(value))
		{
			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : array->build(entryPoint, Stack()))
			{
				auto forwardedValues = values;

				for (auto &resultValue : resultValues)
				{
					forwardedValues.push(resultValue);
				}

				for (auto &result : digest(resultEntryPoint, forwardedValues))
				{
					results.push_back(result);
				}
			}

			return results;
		}

		if (auto tuple = dynamic_cast<const TupleValue *>(value))
		{
			std::vector<DzResult> results;

			auto digestedResults = digest(entryPoint
				, tuple->values()
				);

			for (auto &[digestedEntryPoint, digestedValues] : digestedResults)
			{
				auto digestedTuple = new TupleValue(tuple->iteratorType()
					, { digestedValues.begin(), digestedValues.end() }
					);

				for (auto &[resultEntryPoint, resultValues] : digest(digestedEntryPoint, values))
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

		for (auto &[resultEntryPoint, resultValues] : digest(entryPoint, values))
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

	for (auto &[resultEntryPoint, resultValues] : digest(entryPoint, values))
	{
		for (auto &result : m_consumer->build(resultEntryPoint, resultValues))
		{
			results.push_back(result);
		}
	}

	return results;
}
