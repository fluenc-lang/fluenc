#include <unordered_map>

#include <llvm/IR/Instructions.h>

#include "IRBuilderEx.h"
#include "Junction.h"
#include "Type.h"

#include "types/BuiltinType.h"
#include "types/UserType.h"

#include "values/TypedValue.h"
#include "values/TupleValue.h"
#include "values/TaintedValue.h"
#include "values/UserTypeValue.h"
#include "values/ExpandableValue.h"
#include "values/NamedValue.h"
#include "values/ReferenceValue.h"
#include "values/LazyValue.h"
#include "values/WithoutValue.h"

Junction::Junction(DzValue *subject)
	: m_subject(subject)
{
}

std::vector<DzResult> Junction::build(const EntryPoint &entryPoint, Stack values) const
{
	auto inputResults = m_subject->build(entryPoint, values);

	std::vector<DzResult> outputResults;

	std::multimap<const Type *, SingleResult> groupedResults;

	for (auto &result : inputResults)
	{
		auto [resultEntryPoint, resultValues] = result;

		if (resultValues.size() != 1)
		{
			throw new std::exception();
		}

		auto value = resultValues.pop();

		groupedResults.insert({ value->type(), { resultEntryPoint, value } });
	}

	for (auto it = begin(groupedResults)
		 ; it != end(groupedResults)
		 ; it = upper_bound(it, end(groupedResults), *it, &compareKey<const Type *, SingleResult>)
		 )
	{
		auto [type, _] = *it;

		auto range = groupedResults.equal_range(type);

		std::vector<SingleResult> inputValues;

		std::transform(range.first, range.second, std::back_inserter(inputValues), [](auto pair)
		{
			return pair.second;
		});

		auto [joinedEntryPoint, joinedValue] = tryJoin(inputValues, entryPoint);

		auto finalValues = values;

		finalValues.push(joinedValue);

		outputResults.push_back({ joinedEntryPoint, finalValues });
	}

	return outputResults;
}

const BaseValue *Junction::join(const std::vector<Junction::SingleResult> &range, const EntryPoint &entryPoint) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto function = entryPoint.function();

	auto [_, first] = *range.begin();

	if (auto templateValue = dynamic_cast<const TypedValue *>(first))
	{
		auto type = templateValue->type();
		auto storageType = type->storageType(*context);

		auto alloc = entryPoint.alloc(storageType);

		for (auto &[resultEntryPoint, value] : range)
		{
			auto typedValue = static_cast<const TypedValue *>(value);

			auto resultBlock = resultEntryPoint.block();

			insertBlock(resultBlock, function);

			IRBuilderEx resultBuilder(resultEntryPoint);

			auto junctionStore = resultBuilder.createStore(*typedValue, alloc);

			UNUSED(junctionStore);
		}

		IRBuilderEx junctionBuilder(entryPoint);

		auto junctionLoad = junctionBuilder.createLoad(alloc, "junctionLoad");

		return new TypedValue { type, junctionLoad };
	}
	if (auto templateValue = dynamic_cast<const ReferenceValue *>(first))
	{
		auto type = templateValue->type();
		auto storageType = type->storageType(*context);

		auto alloc = entryPoint.alloc(storageType);

		auto dataLayout = module->getDataLayout();

		for (auto &[resultEntryPoint, value] : range)
		{
			auto referenceValue = static_cast<const ReferenceValue *>(value);

			auto resultBlock = resultEntryPoint.block();

			insertBlock(resultBlock, function);

			IRBuilderEx resultBuilder(resultEntryPoint);

			auto load = resultBuilder.createLoad(*referenceValue, "load");

			resultBuilder.createStore(load, alloc);
		}

		return new ReferenceValue { type, alloc };
	}
	else if (auto templateValue = dynamic_cast<const UserTypeValue *>(first))
	{
		std::vector<const NamedValue *> joinedFieldValues;

		auto fields = templateValue->fields();

		for (auto i = 0u; i < fields.size(); i++)
		{
			std::vector<SingleResult> values;

			std::transform(begin(range), end(range), std::back_inserter(values), [=](auto result) -> SingleResult
			{
				auto [resultEntryPoint, value] = result;

				auto instance = static_cast<const UserTypeValue *>(value);

				auto instanceFields = instance->fields();
				auto instanceField = instanceFields.begin() + i;

				return { resultEntryPoint, *instanceField };
			});

			auto joinedFieldValue = join(values, entryPoint);
			auto joinedNamedValue = static_cast<const NamedValue *>(joinedFieldValue);

			joinedFieldValues.push_back(joinedNamedValue);
		}

		return new UserTypeValue(templateValue->prototype(), joinedFieldValues);
	}
	else if (auto templateValue = dynamic_cast<const TaintedValue *>(first))
	{
		std::vector<SingleResult> values;

		std::transform(begin(range), end(range), std::back_inserter(values), [](auto result) -> SingleResult
		{
			auto [resultEntryPoint, value] = result;

			auto taintedValue = static_cast<const TaintedValue *>(value);

			return { resultEntryPoint, taintedValue->subject() };
		});

		auto joinedValue = join(values, entryPoint);

		return new TaintedValue(joinedValue);

	}
	else if (auto templateValue = dynamic_cast<const TupleValue *>(first))
	{
		std::vector<const BaseValue *> joinedElementValues;

		for (auto i = 0u; i < templateValue->size(); i++)
		{
			std::vector<SingleResult> values;

			std::transform(begin(range), end(range), std::back_inserter(values), [=](auto result) -> SingleResult
			{
				auto [resultEntryPoint, value] = result;

				auto tupleValue = static_cast<const TupleValue *>(value);

				auto elementValues = tupleValue->values();
				auto elementValue = elementValues.begin() + i;

				return { resultEntryPoint, *elementValue };
			});

			auto joinedElementValue = join(values, entryPoint);

			joinedElementValues.push_back(joinedElementValue);
		}

		return new TupleValue(templateValue->iteratorType(), joinedElementValues);
	}
	else if (auto expandableValue = dynamic_cast<const ExpandableValue *>(first))
	{
		return expandableValue;
	}
	else if (auto templateValue = dynamic_cast<const NamedValue *>(first))
	{
		std::vector<SingleResult> values;

		std::transform(begin(range), end(range), std::back_inserter(values), [](auto result) -> SingleResult
		{
			auto [resultEntryPoint, value] = result;

			auto namedValue = static_cast<const NamedValue *>(value);

			return { resultEntryPoint, namedValue->value() };
		});

		auto joined = join(values, entryPoint);

		return new NamedValue(templateValue->name(), joined);
	}
	else if (auto withoutValue = dynamic_cast<const WithoutValue *>(first))
	{
		return withoutValue;
	}

	throw new std::exception();
}

Junction::SingleResult Junction::tryJoin(const std::vector<Junction::SingleResult> &values, const EntryPoint &entryPoint) const
{
	if (values.size() > 1)
	{
		auto &context = entryPoint.context();

		auto junctionBlock = llvm::BasicBlock::Create(*context, "junction");

		auto junctionEntryPoint = entryPoint
			.withBlock(junctionBlock);

		auto joinedValue = join(values, junctionEntryPoint);

		for (auto &[valueEntryPoint, _] : values)
		{
			linkBlocks(valueEntryPoint.block(), junctionBlock);
		}

		return { junctionEntryPoint, joinedValue };
	}

	return values[0];
}
