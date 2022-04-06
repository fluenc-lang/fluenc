#include "ArrayValue.h"
#include "ScalarValue.h"
#include "TupleValue.h"
#include "ReferenceValue.h"
#include "ExpandableValue.h"
#include "IRBuilderEx.h"
#include "IteratorValue.h"
#include "ValueHelper.h"
#include "LazyValue.h"

#include "nodes/TerminatorNode.h"
#include "nodes/LazyEvaluationNode.h"

#include "types/IteratorType.h"
#include "types/Int64Type.h"

ArrayValue::ArrayValue(const ReferenceValue *index
	, const Node *iterator
	, const std::vector<DzResult> &values
	)
	: m_index(index)
	, m_iterator(iterator)
	, m_values(values)
{
}

std::vector<DzResult> ArrayValue::build(const EntryPoint &entryPoint) const
{
	auto &context = entryPoint.context();

	auto block = entryPoint.block();
	auto function = entryPoint.function();

	insertBlock(block, function);

	std::vector<DzResult> results;

	for (auto [_, elementValues] : m_values)
	{
		elementValues.push(m_index);

		insertBlock(block, function);

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
