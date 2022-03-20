#include "LazyEvaluation.h"

#include "values/LazyValue.h"
#include "values/ArrayValue.h"
#include "values/TupleValue.h"
#include "values/IteratorValue.h"

LazyEvaluation::LazyEvaluation(const DzValue *consumer)
	: m_consumer(consumer)
{
}

std::vector<DzResult> digestDepth(const EntryPoint &entryPoint, Stack values)
{
	for (auto i = 0u; i < values.size(); i++)
	{
		auto value = values.pop();

		if (auto iterator = dynamic_cast<const IteratorValue *>(value))
		{
			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : iterator->build(entryPoint))
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

		if (auto lazy = dynamic_cast<const LazyValue *>(value))
		{
			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : lazy->build(entryPoint, Stack()))
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

		if (auto array = dynamic_cast<const ArrayValue *>(value))
		{
			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : array->build(entryPoint))
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

std::vector<DzResult> digestBreadth(const EntryPoint &entryPoint, Stack values)
{
	if (values.size() > 0)
	{
		auto value = values.pop();

		if (auto iterator = dynamic_cast<const IteratorValue *>(value))
		{
			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : iterator->build(entryPoint))
			{
				for (auto &[digestedEntryPoint, digestedValues] : digestBreadth(resultEntryPoint, values))
				{
					auto forwardedValues = resultValues;

					for (auto &resultValue : digestedValues)
					{
						forwardedValues.push(resultValue);
					}

					// So - we could probably *always* call digestBreadth, and remove digestDepth completely.
					// I would just have to figure out how to solve the TupleValue case.

					for (auto &result : digestDepth(digestedEntryPoint, forwardedValues))
					{
						results.push_back(result);
					}
				}
			}

			return results;
		}

		if (auto lazy = dynamic_cast<const LazyValue *>(value))
		{
			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : lazy->build(entryPoint, Stack()))
			{
				for (auto &[digestedEntryPoint, digestedValues] : digestBreadth(resultEntryPoint, values))
				{
					auto forwardedValues = resultValues;

					for (auto &resultValue : digestedValues)
					{
						forwardedValues.push(resultValue);
					}

					for (auto &result : digestDepth(digestedEntryPoint, forwardedValues))
					{
						results.push_back(result);
					}
				}
			}

			return results;
		}

		if (auto array = dynamic_cast<const ArrayValue *>(value))
		{
			std::vector<DzResult> results;

			for (auto &[resultEntryPoint, resultValues] : array->build(entryPoint))
			{
				for (auto &[digestedEntryPoint, digestedValues] : digestBreadth(resultEntryPoint, values))
				{
					auto forwardedValues = resultValues;

					for (auto &resultValue : digestedValues)
					{
						forwardedValues.push(resultValue);
					}

					for (auto &result : digestDepth(digestedEntryPoint, forwardedValues))
					{
						results.push_back(result);
					}
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

		for (auto &[resultEntryPoint, resultValues] : digestBreadth(entryPoint, values))
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

	for (auto &[resultEntryPoint, resultValues] : digestBreadth(entryPoint, values))
	{
		for (auto &result : m_consumer->build(resultEntryPoint, resultValues))
		{
			results.push_back(result);
		}
	}

	return results;
}
