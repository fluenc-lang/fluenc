#include <sstream>

#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

#include "Return.h"
#include "EntryPoint.h"
#include "Type.h"
#include "Terminator.h"
#include "IRBuilderEx.h"

#include "nodes/ExportedFunctionTerminator.h"

#include "types/IteratorType.h"

#include "values/TypedValue.h"
#include "values/ExpandableValue.h"
#include "values/TupleValue.h"

Return::Return(const Type *iteratorType
	, const Node *consumer
	, const Node *chained
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

std::vector<DzResult> Return::build(const EntryPoint &entryPoint, Stack values) const
{
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	insertBlock(block, function);

	auto value = fetchValue(values, entryPoint);

	if (m_chained)
	{
		auto continuation = new ExpandableValue(m_iteratorType, entryPoint, m_chained);
		auto tuple = new TupleValue(m_iteratorType, { continuation, value });

		values.push(tuple);

		return m_consumer->build(entryPoint, values);
	}

	values.push(value);

	return m_consumer->build(entryPoint, values);
}
