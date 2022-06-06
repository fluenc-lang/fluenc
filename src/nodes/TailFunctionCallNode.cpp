#include <numeric>

#include "AllIterator.h"
#include "ZipIterator.h"
#include "ValueHelper.h"
#include "FunctionHelper.h"

#include "nodes/TailFunctionCallNode.h"
#include "nodes/JunctionNode.h"
#include "nodes/CallableNode.h"
#include "nodes/CapsuleNode.h"

#include "values/LazyValue.h"
#include "values/IteratorValueGenerator.h"

#include "types/IteratorType.h"

#include "iterators/ExtremitiesIterator.h"

TailFunctionCallNode::TailFunctionCallNode(const std::vector<std::string> &names, const Node *regularCall)
	: m_names(names)
	, m_regularCall(regularCall)
{
}

std::vector<DzResult> TailFunctionCallNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	insertBlock(block, function);

	auto [score, tailCallTarget, targetValues] = FunctionHelper::tryCreateTailCall(entryPoint, values, begin(m_names), end(m_names));

	if (score == 0)
	{
		auto zipped = zip(values, targetValues);

		auto resultEntryPoint = std::accumulate(zipped.begin(), zipped.end(), entryPoint, [&](auto accumulatedEntryPoint, auto result)
		{
			auto [value, storage] = result;

			return ValueHelper::transferValue(accumulatedEntryPoint, value, storage);
		});

		linkBlocks(resultEntryPoint.block(), tailCallTarget->block());

		return std::vector<DzResult>();
	}

	if (score == 1)
	{
		throw new std::exception(); // TODO
	}

	auto junction = new JunctionNode(m_regularCall);

	return junction->build(entryPoint, values);
}

int TailFunctionCallNode::order(const EntryPoint &entryPoint) const
{
	return m_regularCall->order(entryPoint);
}
