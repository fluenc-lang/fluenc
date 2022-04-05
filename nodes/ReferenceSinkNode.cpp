#include "ReferenceSinkNode.h"
#include "IRBuilderEx.h"

#include "values/TupleValue.h"
#include "values/NamedValue.h"
#include "values/ScalarValue.h"
#include "values/ReferenceValue.h"
#include "values/UserTypeValue.h"

ReferenceSinkNode::ReferenceSinkNode(const Node *consumer)
	: m_consumer(consumer)
{
}

const BaseValue *ReferenceSinkNode::makeReference(const BaseValue *value, const EntryPoint &entryPoint) const
{
	auto &context = entryPoint.context();

	IRBuilderEx builder(entryPoint);

	if (auto typedValue = dynamic_cast<const ScalarValue *>(value))
	{
		auto argumentType = typedValue->type();
		auto storageType = argumentType->storageType(*context);

		auto alloc = entryPoint.alloc(storageType);

		builder.createStore(*typedValue, alloc);

		return new ReferenceValue { argumentType, alloc };
	}

	if (auto referenceValue = dynamic_cast<const ReferenceValue *>(value))
	{
		auto argumentType = referenceValue->type();
		auto storageType = argumentType->storageType(*context);

		auto load = builder.createLoad(*referenceValue);

		auto alloc = entryPoint.alloc(storageType);

		builder.createStore(load, alloc);

		return new ReferenceValue { argumentType, alloc };
	}

	if (auto tupleValue = dynamic_cast<const TupleValue *>(value))
	{
		auto tupleValues = tupleValue->values();

		std::vector<const BaseValue *> values;

		std::transform(tupleValues.begin(), tupleValues.end(), std::back_inserter(values), [&](auto value)
		{
			return makeReference(value, entryPoint);
		});

		return new TupleValue(tupleValue->iteratorType(), values);
	}

	if (auto userTypeValue = dynamic_cast<const UserTypeValue *>(value))
	{
		auto fields = userTypeValue->fields();

		std::vector<const NamedValue *> values;

		std::transform(begin(fields), end(fields), std::back_inserter(values), [&](auto field)
		{
			return new NamedValue { field->name(), makeReference(field->value(), entryPoint) };
		});

		return new UserTypeValue { userTypeValue->prototype(), values };
	}

	return value;
}

std::vector<DzResult> ReferenceSinkNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto value = makeReference(values.pop(), entryPoint);

	values.push(value);

	return m_consumer->build(entryPoint, values);
}
