#include <unordered_map>

#include <llvm/IR/Instructions.h>

#include "IRBuilderEx.h"
#include "nodes/JunctionNode.h"
#include "Type.h"
#include "ValueHelper.h"

#include "types/BuiltinType.h"
#include "types/UserType.h"

#include "values/ScalarValue.h"
#include "values/TupleValue.h"
#include "values/UserTypeValue.h"
#include "values/ExpandableValue.h"
#include "values/NamedValue.h"
#include "values/ReferenceValue.h"
#include "values/IteratorValue.h"
#include "values/WithoutValue.h"
#include "values/LazyValue.h"
#include "values/StringValue.h"

JunctionNode::JunctionNode(const Node *subject)
	: m_subject(subject)
{
}

std::vector<DzResult> JunctionNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto inputResults = m_subject->build(entryPoint, Stack());

	std::vector<DzResult> outputResults;

	std::multimap<const Type *, SingleResult> groupedResults;

	for (auto &result : inputResults)
	{
		auto [resultEntryPoint, resultValues] = result;

		if (resultValues.size() != 1)
		{
			outputResults.push_back(result);

			continue;
		}

		auto value = resultValues.pop();

		groupedResults.insert({ value->type(), { resultEntryPoint, value } });
	}

	for (auto it = begin(groupedResults)
		 ; it != end(groupedResults)
		 ; it = upper_bound(it, end(groupedResults), *it, &compareKey<const Type *, SingleResult>)
		 )
	{
		auto [type, _] = *it;

		auto range = groupedResults.equal_range(type);

		std::vector<SingleResult> inputValues;

		std::transform(range.first, range.second, std::back_inserter(inputValues), [](auto pair)
		{
			return pair.second;
		});

		auto [joinedEntryPoint, joinedValue] = tryJoin(inputValues, entryPoint);

		auto finalValues = values;

		finalValues.push(joinedValue);

		outputResults.push_back({ joinedEntryPoint, finalValues });
	}

	return outputResults;
}

const BaseValue *JunctionNode::join(const std::vector<JunctionNode::SingleResult> &range, const EntryPoint &entryPoint) const
{
	auto function = entryPoint.function();

	auto [_, first] = *range.begin();

	if (auto templateValue = dynamic_cast<const ScalarValue *>(first))
	{
		auto type = templateValue->type();

		auto alloc = entryPoint.alloc(type);

		for (auto &[resultEntryPoint, value] : range)
		{
			auto transferEntryPoint = ValueHelper::transferValue(resultEntryPoint, value, alloc);

			insertBlock(transferEntryPoint.block(), function);
			linkBlocks(transferEntryPoint.block(), entryPoint.block());
		}

		IRBuilderEx junctionBuilder(entryPoint);

		return junctionBuilder.createLoad(alloc, "junctionLoad");
	}

	auto alloc = first->clone(entryPoint);

	for (auto &[resultEntryPoint, value] : range)
	{
		auto transferEntryPoint = ValueHelper::transferValue(resultEntryPoint, value, alloc);

		insertBlock(transferEntryPoint.block(), function);
		linkBlocks(transferEntryPoint.block(), entryPoint.block());
	}

	return alloc;
}

JunctionNode::SingleResult JunctionNode::tryJoin(const std::vector<JunctionNode::SingleResult> &values, const EntryPoint &entryPoint) const
{
	if (values.size() > 1)
	{
		auto &context = entryPoint.context();

		auto junctionBlock = llvm::BasicBlock::Create(*context, "junction");

		auto junctionEntryPoint = entryPoint
			.withBlock(junctionBlock);

		auto joined = join(values, junctionEntryPoint);

		return { junctionEntryPoint, joined };
	}

	return values[0];
}
