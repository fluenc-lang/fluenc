#include <numeric>

#include <range/v3/view.hpp>

#include "ValueHelper.h"
#include "IRBuilderEx.h"
#include "ITransferStrategy.h"
#include "Utility.h"

#include "values/ReferenceValue.h"
#include "values/UserTypeValue.h"
#include "values/LazyValue.h"
#include "values/NamedValue.h"
#include "values/ScalarValue.h"
#include "values/TupleValue.h"
#include "values/StringValue.h"
#include "values/ExpandedValue.h"
#include "values/ExpandableValue.h"
#include "values/BufferValue.h"

EntryPoint ValueHelper::transferValue(const EntryPoint &entryPoint
	, const BaseValue *value
	, const BaseValue *storage
	, const Emitter &emitter
	)
{
	static std::vector<const ITransferStrategy *> strategies =
	{
		strategyFor<ScalarValue, ReferenceValue>([](auto entryPoint, auto source, auto target, auto& emitter)
		{
			UNUSED(emitter);

			IRBuilderEx builder(entryPoint);

			builder.createStore(source, target);

			return entryPoint;
		}),
		strategyFor<ReferenceValue, ReferenceValue>([](auto entryPoint, auto source, auto target, auto& emitter)
		{
			UNUSED(emitter);

			IRBuilderEx builder(entryPoint);

			auto load = builder.createLoad(source);

			builder.createStore(load, target);

			return entryPoint;
		}),
		strategyFor<LazyValue, LazyValue>([](auto entryPoint, auto source, auto target, auto& emitter)
		{
			return target->assignFrom(entryPoint, source, emitter);
		}),
		strategyFor<BaseValue, LazyValue>([](auto entryPoint, auto source, auto target, auto& emitter)
		{
			return target->assignFrom(entryPoint, source, emitter);
		}),
		strategyFor<UserTypeValue, UserTypeValue>([](auto entryPoint, auto source, auto target, auto& emitter)
		{
			auto valueFields = source->fields();
			auto storageFields = target->fields();

			auto zipped = ranges::views::zip(valueFields, storageFields);

			return std::accumulate(zipped.begin(), zipped.end(), entryPoint, [&](auto accumulatedEntryPoint, auto fields)
			{
				auto [valueField, storageField] = fields;

				return transferValue(accumulatedEntryPoint
					, valueField->value()
					, storageField->value()
					, emitter
					);
			});
		}),
		strategyFor<TupleValue, TupleValue>([](auto entryPoint, auto source, auto target, auto& emitter)
		{
			auto valueElements = source->values();
			auto storageElements = target->values();

			auto zipped = ranges::views::zip(valueElements, storageElements);

			return std::accumulate(zipped.begin(), zipped.end(), entryPoint, [&](auto accumulatedEntryPoint, auto elements)
			{
				auto [valueElement, storageElement] = elements;

				return transferValue(accumulatedEntryPoint
					, valueElement
					, storageElement
					, emitter
					);
			});
		}),
		strategyFor<StringValue, StringValue>([](auto entryPoint, auto source, auto target, auto& emitter)
		{
			UNUSED(emitter);

			IRBuilderEx builder(entryPoint);

			auto referenceLoad = builder.createLoad(source->reference());
			auto lengthLoad = builder.createLoad(source->length());

			builder.createStore(referenceLoad, target->reference());
			builder.createStore(lengthLoad, target->length());

			return entryPoint;
		}),
		strategyFor<ExpandedValue, ExpandableValue>([](auto entryPoint, auto source, auto target, auto& emitter)
		{
			auto sourceValues = source->values();
			auto targetValues = target->values();

			auto zipped = ranges::views::zip(sourceValues, targetValues);

			return std::accumulate(zipped.begin(), zipped.end(), entryPoint, [&](auto accumulatedEntryPoint, auto elements)
			{
				auto [sourceValue, targetValue] = elements;

				return transferValue(accumulatedEntryPoint
					, sourceValue
					, targetValue
					, emitter
					);
			});
		}),
		strategyFor<BufferValue, BufferValue>([](auto entryPoint, auto source, auto target, auto& emitter)
		{
			UNUSED(emitter);

			IRBuilderEx builder(entryPoint);

			auto referenceLoad = builder.createLoad(source->reference(entryPoint));

			builder.createStore(referenceLoad, target->reference(entryPoint));

			return entryPoint;
		}),
	};

	auto resultEntryPoint = std::accumulate(begin(strategies), end(strategies), std::optional<EntryPoint>(), [&](auto accumulated, auto strategy)
	{
		if (accumulated)
		{
			return accumulated;
		}

		return strategy->invoke(entryPoint, value, storage, emitter);
	});

	return resultEntryPoint.value_or(entryPoint);
}

const ScalarValue *ValueHelper::getScalar(const std::shared_ptr<peg::Ast> &ast, const EntryPoint &entryPoint, const BaseValue *value)
{
	if (auto scalar = value_cast<const ScalarValue *>(value))
	{
		return scalar;
	}

	if (auto reference = value_cast<const ReferenceValue *>(value))
	{
		IRBuilderEx builder(entryPoint);

		return builder.createLoad(reference);
	}

	auto &expectedMetadata = ScalarValue::staticMetadata();
	auto &actualMetadata = value->metadata();

	throw InvalidTypeException(ast
		, expectedMetadata.name()
		, actualMetadata.name()
		);
}

const ScalarValue *ValueHelper::getScalar(const std::shared_ptr<peg::Ast> &ast, const EntryPoint &entryPoint, Stack &values)
{
	auto value = values.pop();

	return getScalar(ast, entryPoint, value);
}
