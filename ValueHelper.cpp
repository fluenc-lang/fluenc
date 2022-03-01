#include <numeric>

#include "ValueHelper.h"
#include "IRBuilderEx.h"
#include "ZipIterator.h"

#include "values/ReferenceValue.h"
#include "values/UserTypeValue.h"
#include "values/ArrayValue.h"
#include "values/LazyValue.h"
#include "values/NamedValue.h"
#include "values/TypedValue.h"

EntryPoint ValueHelper::transferValue(const EntryPoint &entryPoint
	, const BaseValue *value
	, const BaseValue *storage
	)
{
	if (auto typedValue = dynamic_cast<const TypedValue *>(value))
	{
		IRBuilderEx builder(entryPoint);

		builder.createStore(*typedValue, *dynamic_cast<const ReferenceValue *>(storage));
	}
	else if (auto reference = dynamic_cast<const ReferenceValue *>(value))
	{
		IRBuilderEx builder(entryPoint);

		auto load = builder.createLoad(*reference);

		builder.createStore(load, *dynamic_cast<const ReferenceValue *>(storage));
	}
	else if (auto userTypeValue = dynamic_cast<const UserTypeValue *>(value))
	{
		auto userTypeStorage = dynamic_cast<const UserTypeValue *>(storage);

		auto valueFields = userTypeValue->fields();
		auto storageFields = userTypeStorage->fields();

		auto zipped = zip(valueFields, storageFields);

		return std::accumulate(zipped.begin(), zipped.end(), entryPoint, [&](auto accumulatedEntryPoint, auto fields)
		{
			auto [valueField, storageField] = fields;

			return transferValue(accumulatedEntryPoint
				, valueField->value()
				, storageField->value()
				);
		});
	}
	else if (auto arrayValue = dynamic_cast<const ArrayValue *>(value))
	{
		auto arrayStorage = dynamic_cast<const ArrayValue *>(storage);

		return arrayStorage->assignFrom(entryPoint, arrayValue);
	}
	else if (auto lazyValue = dynamic_cast<const LazyValue *>(value))
	{
		auto arrayStorage = dynamic_cast<const ArrayValue *>(storage);

		return arrayStorage->assignFrom(entryPoint, lazyValue);
	}

	return entryPoint;
}
