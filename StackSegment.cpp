#include <llvm/IR/IRBuilder.h>

#include <numeric>

#include "StackSegment.h"
#include "Type.h"

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

	auto orderedValues = m_values;

	std::sort(begin(orderedValues), end(orderedValues), [=](auto first, auto second)
	{
		return first->compare(second, entryPoint);
	});

	auto subjectResults = std::accumulate(begin(orderedValues), end(orderedValues), input, [=](auto accumulator, auto value)
	{
		std::vector<DzResult> results;

		for (auto &[accumulatorEntryPoint, accumulatorValues] : accumulator)
		{
			auto result = value->build(accumulatorEntryPoint, accumulatorValues);

			results.insert(end(results), begin(result), end(result));
		}

		return results;
	});

	for (auto &[subjectEntryPoint, subjectValues] : subjectResults)
	{
		auto block = subjectEntryPoint.block();

		Stack pointersToValues;

		for (auto &value : subjectValues)
		{
			auto argumentType = value.type();
			auto storageType = argumentType->storageType(*context);

			auto alloc = entryPoint.alloc(storageType);

			auto align = dataLayout.getABITypeAlign(storageType);

			auto store = new llvm::StoreInst(value, alloc, false, align, block);

			UNUSED(store);

			pointersToValues.push({ argumentType, alloc });
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
