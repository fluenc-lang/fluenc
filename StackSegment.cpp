#include <llvm/IR/IRBuilder.h>

#include <numeric>

#include "StackSegment.h"
#include "Type.h"
#include "IndexIterator.h"
#include "Indexed.h"

#include "values/IndexedValue.h"
#include "values/TypedValue.h"
#include "values/TupleValue.h"
#include "values/ReferenceValue.h"

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

const BaseValue *fetchValue(const BaseValue *value, const EntryPoint &entryPoint)
{
	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto dataLayout = module->getDataLayout();

	auto block = entryPoint.block();

	if (auto typedValue = dynamic_cast<const TypedValue *>(value))
	{
		auto argumentType = typedValue->type();
		auto storageType = argumentType->storageType(*context);

		auto alloc = entryPoint.alloc(storageType);

		auto align = dataLayout.getABITypeAlign(storageType);

		auto store = new llvm::StoreInst(*typedValue, alloc, false, align, block);

		UNUSED(store);

		return new ReferenceValue { argumentType, alloc };
	}

	if (auto tupleValue = dynamic_cast<const TupleValue *>(value))
	{
		auto tupleValues = tupleValue->values();

		std::vector<const BaseValue *> values;

		std::transform(tupleValues.begin(), tupleValues.end(), std::back_insert_iterator(values), [&](auto value)
		{
			return fetchValue(value, entryPoint);
		});

		return new TupleValue { values };
	}

	return value;
}

std::vector<DzResult> StackSegment::build(const EntryPoint &entryPoint, Stack values) const
{
	std::vector<DzResult> result;
	std::vector<DzResult> input = {{ entryPoint, Stack() }};

	std::vector<Indexed<DzValue *>> orderedValues;

	std::transform(begin(m_values), end(m_values), index_iterator(), std::back_insert_iterator(orderedValues), [](auto value, auto index) -> Indexed<DzValue *>
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
					auto returnValue = new IndexedValue{ argument.index, fetchValue(resultValue, resultEntryPoint) };

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
