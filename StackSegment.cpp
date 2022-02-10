#include <llvm/IR/IRBuilder.h>

#include <numeric>

#include "StackSegment.h"
#include "Type.h"
#include "IndexIterator.h"
#include "Indexed.h"
#include "IRBuilderEx.h"

#include "values/IndexedValue.h"
#include "values/TypedValue.h"
#include "values/TupleValue.h"
#include "values/ReferenceValue.h"
#include "values/UserTypeValue.h"
#include "values/NamedValue.h"

StackSegment::StackSegment(std::vector<DzValue *> values, DzValue *call, DzValue *consumer)
	: m_values(values)
	, m_call(call)
	, m_consumer(consumer)
{
}

int StackSegment::order(const EntryPoint &entryPoint) const
{
	return m_call->order(entryPoint);
}

std::vector<DzResult> StackSegment::build(const EntryPoint &entryPoint, Stack values) const
{
	std::vector<DzResult> result;
	std::vector<DzResult> input = {{ entryPoint, Stack() }};

	std::vector<Indexed<DzValue *>> orderedValues;

	std::transform(begin(m_values), end(m_values), index_iterator(), std::back_inserter(orderedValues), [](auto value, auto index) -> Indexed<DzValue *>
	{
		return { index, value };
	});

	std::sort(begin(orderedValues), end(orderedValues), [=](auto first, auto second)
	{
		return first.value->order(entryPoint) < second.value->order(entryPoint);
	});

	auto subjectResults = std::accumulate(begin(orderedValues), end(orderedValues), input, [&](auto accumulator, auto argument)
	{
		std::vector<DzResult> results;

		for (auto &[accumulatorEntryPoint, accumulatorValues] : accumulator)
		{
			auto result = argument.value->build(accumulatorEntryPoint, Stack());

			for (auto &[resultEntryPoint, resultValues] : result)
			{
				auto scopedReturnValues = accumulatorValues;

				for (auto resultValue : resultValues)
				{
					auto returnValue = new IndexedValue{ argument.index, resultValue };

					scopedReturnValues.push(returnValue);
				}

				results.push_back({ resultEntryPoint, scopedReturnValues });
			}
		}

		return results;
	});

	for (auto &[subjectEntryPoint, subjectValues] : subjectResults)
	{
		std::multimap<int, const BaseValue *, std::greater<int>> indexedValues;

		for (auto &value : subjectValues)
		{
			auto indexed = static_cast<const IndexedValue *>(value);

			indexedValues.insert({ indexed->index(), indexed->subject() });
		}

		Stack pointersToValues;

		for (auto [_, value] : indexedValues)
		{
			pointersToValues.push(value);
		}

		auto callResults = m_call->build(subjectEntryPoint, pointersToValues);

		for (auto &[callEntryPoint, callValues] : callResults)
		{
			auto forwardedValues = values;

			for (auto &value : callValues)
			{
				forwardedValues.push(value);
			}

			auto consumerResults = m_consumer->build(callEntryPoint, forwardedValues);

			for (auto &consumerResult : consumerResults)
			{
				result.push_back(consumerResult);
			}
		}
	}

	return result;
}
