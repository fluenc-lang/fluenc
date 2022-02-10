#include <sstream>

#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

#include "DzReturn.h"
#include "EntryPoint.h"
#include "Type.h"
#include "DzExportedFunctionTerminator.h"
#include "DzTerminator.h"
#include "IRBuilderEx.h"

#include "types/IteratorType.h"

#include "values/TypedValue.h"
#include "values/ExpandableValue.h"
#include "values/TupleValue.h"

DzReturn::DzReturn(const Type *iteratorType
	, const DzValue *consumer
	, const DzValue *chained
	)
	: m_iteratorType(iteratorType)
	, m_consumer(consumer)
	, m_chained(chained)
{
}

const BaseValue *fetchValue(Stack &values, const EntryPoint &entryPoint)
{
	auto value = values.pop();

	if (auto typedValue = dynamic_cast<const TypedValue *>(value))
	{
		auto &context = entryPoint.context();

		IRBuilderEx builder(entryPoint);

		auto type = value->type();
		auto storageType = type->storageType(*context);

		auto alloc = entryPoint.alloc(storageType);

		builder.createStore(*typedValue, alloc);

		auto load = builder.createLoad(alloc);

		return new TypedValue { type, load };
	}

	return value;
}

std::vector<DzResult> DzReturn::build(const EntryPoint &entryPoint, Stack values) const
{
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	block->insertInto(function);

	auto value = fetchValue(values, entryPoint);

	if (m_chained)
	{
		auto continuation = new ExpandableValue(m_iteratorType, entryPoint, m_chained);
		auto tuple = new TupleValue(m_iteratorType, { continuation, value });

		values.push(tuple);

		return m_consumer->build(entryPoint, values);
	}

	values.push(value);

	if (dynamic_cast<const TupleValue *>(value))
	{
		return m_consumer->build(entryPoint, values);
	}

	return m_consumer->build(entryPoint, values);
}
