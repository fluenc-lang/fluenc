#include <llvm/IR/IRBuilder.h>

#include <sstream>

#include "DzInstantiation.h"
#include "DzTypeName.h"
#include "IndexIterator.h"
#include "IPrototypeProvider.h"

#include "types/Prototype.h"

#include "values/TypedValue.h"
#include "values/NamedValue.h"
#include "values/UserTypeValue.h"
#include "values/ReferenceValue.h"

DzInstantiation::DzInstantiation(DzValue *consumer
	, IPrototypeProvider *prototypeProvider
	, const std::vector<std::string> &fields
	)
	: m_consumer(consumer)
	, m_prototypeProvider(prototypeProvider)
	, m_fields(fields)
{
}

std::vector<DzResult> DzInstantiation::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	std::unordered_map<std::string, const BaseValue *> valuesByName;

	std::transform(begin(m_fields), end(m_fields), std::insert_iterator(valuesByName, begin(valuesByName)), [&](auto field)
	{
		return std::make_pair(field, values.pop());
	});

	auto prototype = m_prototypeProvider->provide(entryPoint, values);

	auto prototypeFields = prototype->fields(entryPoint);

	std::vector<const NamedValue *> namedValues;

	std::transform(begin(prototypeFields), end(prototypeFields), std::back_insert_iterator(namedValues), [&](auto field) -> const NamedValue *
	{
		auto valueByName = valuesByName.find(field.name());

		if (valueByName != valuesByName.end())
		{
			auto value = valueByName->second;

			if (auto reference = dynamic_cast<const ReferenceValue *>(field.defaultValue()))
			{
				if (reference->type() != value->type())
				{
					return new NamedValue { field.name(), value };
				}

				auto type = reference->type();
				auto storageType = type->storageType(*context);

				auto align = dataLayout.getABITypeAlign(storageType);

				auto typedValue = static_cast<const TypedValue *>(value);

				auto store = new llvm::StoreInst(*typedValue, *reference, false, align, block);

				UNUSED(store);

				return new NamedValue { field.name(), field.defaultValue() };
			}

			return new NamedValue { field.name(), value };
		}

		if (!field.defaultValue())
		{
			throw new std::exception();
		}

		return new NamedValue { field.name(), field.defaultValue() };
	});

	std::vector<const NamedValue *> finalValues;

	std::transform(begin(namedValues), end(namedValues), std::back_insert_iterator(finalValues), [&](auto field) -> const NamedValue *
	{
		auto value = field->value();

		if (auto typedValue = dynamic_cast<const TypedValue *>(value))
		{
			auto type = typedValue->type();
			auto storageType = type->storageType(*context);

			auto align = dataLayout.getABITypeAlign(storageType);

			auto alloc = entryPoint.alloc(storageType);

			auto store = new llvm::StoreInst(*typedValue, alloc, false, align, block);

			UNUSED(store);

			auto reference = new ReferenceValue { type, alloc };

			return new NamedValue { field->name(), reference };
		}

		return field;
	});

	auto userTypeValue = new UserTypeValue(prototype, finalValues);

	values.push(userTypeValue);

	return m_consumer->build(entryPoint, values);
}
