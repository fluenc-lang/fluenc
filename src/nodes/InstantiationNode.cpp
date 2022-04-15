#include <llvm/IR/IRBuilder.h>

#include <sstream>
#include <unordered_map>

#include "nodes/InstantiationNode.h"
#include "DzTypeName.h"
#include "IndexIterator.h"
#include "IPrototypeProvider.h"
#include "IRBuilderEx.h"

#include "types/Prototype.h"

#include "values/ScalarValue.h"
#include "values/NamedValue.h"
#include "values/UserTypeValue.h"
#include "values/ReferenceValue.h"

InstantiationNode::InstantiationNode(const std::vector<std::string> &fields
	, const IPrototypeProvider *prototypeProvider
	, const Node *consumer
	)
	: m_fields(fields)
	, m_prototypeProvider(prototypeProvider)
	, m_consumer(consumer)
{
}

std::vector<DzResult> InstantiationNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	std::unordered_map<std::string, const BaseValue *> valuesByName;

	std::transform(begin(m_fields), end(m_fields), std::inserter(valuesByName, begin(valuesByName)), [&](auto field)
	{
		return std::make_pair(field, values.pop());
	});

	auto prototype = m_prototypeProvider->provide(entryPoint, values);

	auto prototypeFields = prototype->fields(entryPoint);

	std::vector<const NamedValue *> namedValues;

	std::transform(begin(prototypeFields), end(prototypeFields), std::back_inserter(namedValues), [&](auto field) -> const NamedValue *
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

				auto typedValue = static_cast<const ScalarValue *>(value);

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

	IRBuilderEx builder(entryPoint);

	std::vector<const NamedValue *> finalValues;

	std::transform(begin(namedValues), end(namedValues), std::back_inserter(finalValues), [&](auto field) -> const NamedValue *
	{
		auto value = field->value();

		if (auto typedValue = dynamic_cast<const ScalarValue *>(value))
		{
			auto type = typedValue->type();

			auto alloc = entryPoint.alloc(type);

			builder.createStore(typedValue, alloc);

			return new NamedValue { field->name(), alloc };
		}

		return field;
	});

	auto userTypeValue = new UserTypeValue(prototype, finalValues);

	values.push(userTypeValue);

	return m_consumer->build(entryPoint, values);
}
