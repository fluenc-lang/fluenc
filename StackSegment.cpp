#include <llvm/IR/IRBuilder.h>

#include <numeric>

#include "StackSegment.h"
#include "Type.h"
#include "IndexIterator.h"
#include "IndexedValue.h"

StackSegment::StackSegment(std::vector<DzValue *> values, DzValue *call, DzValue *consumer)
	: m_values(values)
	, m_call(call)
	, m_consumer(consumer)
{
}

int StackSegment::compare(DzValue *other, const EntryPoint &entryPoint) const
{
	UNUSED(other);
	UNUSED(entryPoint);

	return -1;
}

std::vector<DzResult> StackSegment::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto dataLayout = module->getDataLayout();

	std::vector<DzResult> result;
	std::vector<DzResult> input = {{ entryPoint, Stack() }};

	std::vector<IndexedValue> orderedValues;

	std::transform(begin(m_values), end(m_values), index_iterator(), std::back_insert_iterator(orderedValues), [](auto value, auto index) -> IndexedValue
	{
		return { index, value };
	});

	std::sort(begin(orderedValues), end(orderedValues), [=](auto first, auto second)
	{
		return first.value->compare(second.value, entryPoint);
	});

	auto subjectResults = std::accumulate(begin(orderedValues), end(orderedValues), input, [=](auto accumulator, auto argument)
	{
		std::multimap<int, DzResult> results;

		for (auto &[accumulatorEntryPoint, accumulatorValues] : accumulator)
		{
			auto result = argument.value->build(accumulatorEntryPoint, accumulatorValues);

			for (auto &r : result)
			{
				results.insert({ argument.index, r });
			}
		}

		std::vector<DzResult> orderedResults;

		std::transform(begin(results), end(results), std::back_insert_iterator(orderedResults), [](auto pair)
		{
			return pair.second;
		});

		return orderedResults;
	});

	for (auto &[subjectEntryPoint, subjectValues] : subjectResults)
	{
		auto block = subjectEntryPoint.block();

		Stack pointersToValues;

		for (auto &value : subjectValues)
		{
			if (auto typedValue = dynamic_cast<const TypedValue *>(value))
			{
				auto argumentType = value->type();
				auto storageType = argumentType->storageType(*context);

				auto alloc = entryPoint.alloc(storageType);

				auto align = dataLayout.getABITypeAlign(storageType);

				auto store = new llvm::StoreInst(*typedValue, alloc, false, align, block);

				UNUSED(store);

				pointersToValues.push(new TypedValue { argumentType, alloc });
			}
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
