#include <sstream>

#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

#include "DzReturn.h"
#include "EntryPoint.h"
#include "Type.h"
#include "DzExportedFunctionTerminator.h"
#include "DzTerminator.h"

#include "types/IteratorType.h"

#include "values/TypedValue.h"
#include "values/ExpandableValue.h"
#include "values/TupleValue.h"

DzReturn::DzReturn(DzValue *consumer, DzValue *chained)
	: m_consumer(consumer)
	, m_chained(chained)
{
}

const BaseValue *fetchValue(Stack &values, const EntryPoint &entryPoint)
{
	auto value = values.pop();

	if (auto typedValue = dynamic_cast<const TypedValue *>(value))
	{
		auto &context = entryPoint.context();

		auto block = entryPoint.block();

		llvm::IRBuilder<> builder(block);

		auto type = value->type();
		auto storageType = type->storageType(*context);

		auto alloc = entryPoint.alloc(storageType);

		builder.CreateStore(*typedValue, alloc);

		auto load = builder.CreateLoad(storageType, alloc);

		return new TypedValue { type, load };
	}

	return value;
}

std::vector<DzResult> DzReturn::build(const EntryPoint &entryPoint, Stack values) const
{
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	block->insertInto(function);

	llvm::IRBuilder<> builder(block);

	auto value = fetchValue(values, entryPoint);

	if (m_chained)
	{
		auto continuation = new ExpandableValue(entryPoint, m_chained);
		auto tuple = new TupleValue({ continuation, value });

		values.push(tuple);

		return m_consumer->build(entryPoint, values);
	}

	values.push(value);

	if (dynamic_cast<const TupleValue *>(value))
	{
		return m_consumer->build(entryPoint, values);
	}

	auto ep = entryPoint
		.withIteratorDepth(entryPoint.iteratorDepth() - 1);

	return m_consumer->build(ep, values);
}
