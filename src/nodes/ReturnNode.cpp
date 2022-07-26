#include <sstream>

#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

#include "nodes/ReturnNode.h"
#include "EntryPoint.h"
#include "Type.h"
#include "nodes/TerminatorNode.h"
#include "IRBuilderEx.h"

#include "nodes/ExportedFunctionTerminatorNode.h"

#include "types/IteratorType.h"

#include "values/ScalarValue.h"
#include "values/ExpandableValue.h"
#include "values/TupleValue.h"
#include "values/ReferenceValue.h"

ReturnNode::ReturnNode(const Type *iteratorType
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

	if (auto typedValue = dynamic_cast<const ScalarValue *>(value))
	{
		IRBuilderEx builder(entryPoint);

		auto type = value->type();

		auto alloc = entryPoint.alloc(type);

		builder.createStore(typedValue, alloc);

		return alloc;
	}

	return value;
}

std::vector<DzResult> ReturnNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	insertBlock(block, function);

	auto value = fetchValue(values, entryPoint);

	if (m_chained)
	{
		auto expandable = new ExpandableValue(false, m_iteratorType, entryPoint, m_chained);

		auto tuple = new TupleValue({ expandable, value });

		values.push(tuple);

		return m_consumer->build(entryPoint, values);
	}

	values.push(value);

	return m_consumer->build(entryPoint, values);
}
