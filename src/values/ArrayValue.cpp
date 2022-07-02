#include "IRBuilderEx.h"
#include "ValueHelper.h"
#include "IndexIterator.h"

#include "values/ArrayValue.h"
#include "values/ScalarValue.h"
#include "values/TupleValue.h"
#include "values/ReferenceValue.h"
#include "values/ExpandableValue.h"
#include "values/IteratorValue.h"
#include "values/LazyValue.h"

#include "nodes/TerminatorNode.h"
#include "nodes/LazyEvaluationNode.h"

#include "types/IteratorType.h"
#include "types/Int64Type.h"

#include "nodes/ArrayElementNode.h"
#include "nodes/IteratableNode.h"

ArrayValue::ArrayValue(const ReferenceValue *index
	, const Type *type
	, const std::vector<DzResult> &values
	, size_t size
	)
	: m_index(index)
	, m_iterator(createIterator(this, type, size))
	, m_values(values)
{
}

std::vector<DzResult> ArrayValue::build(const EntryPoint &entryPoint) const
{
	auto context = entryPoint.context();

	auto block = entryPoint.block();
	auto function = entryPoint.function();

	insertBlock(block, function);

	std::vector<DzResult> results;

	for (auto [_, elementValues] : m_values)
	{
		elementValues.push(m_index);

		auto arrayBlock = llvm::BasicBlock::Create(*context);

		linkBlocks(block, arrayBlock);

		auto iteratorEntryPoint = entryPoint
			.withBlock(arrayBlock)
			.withName("__array")
			.markEntry()
			;

		for (auto &result : m_iterator->build(iteratorEntryPoint, elementValues))
		{
			results.push_back(result);
		}
	}

	return results;
}

const Node *ArrayValue::createIterator(const IIteratable *parent, const Type *type, size_t size)
{
	return std::accumulate(index_iterator(0u), index_iterator(size), (Node *)nullptr, [&](auto next, auto)
	{
		return new ArrayElementNode(type, new IteratableNode(parent), next);
	});
}
