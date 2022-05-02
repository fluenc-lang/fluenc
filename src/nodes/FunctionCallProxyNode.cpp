#include <numeric>

#include "AllIterator.h"
#include "ZipIterator.h"
#include "ValueHelper.h"

#include "nodes/FunctionCallProxyNode.h"
#include "nodes/JunctionNode.h"
#include "nodes/CallableNode.h"
#include "nodes/CapsuleNode.h"

#include "values/LazyValue.h"
#include "values/IteratorValueGenerator.h"

#include "types/IteratorType.h"

#include "iterators/ExtremitiesIterator.h"

FunctionCallProxyNode::FunctionCallProxyNode(const std::vector<std::string> &names, const Node *regularCall)
	: m_names(names)
	, m_regularCall(regularCall)
{
}

int8_t FunctionCallProxyNode::tryCreateTailCall(const EntryPoint &entryPoint
	, const Stack &values
	, const std::vector<std::string>::const_iterator &name
	, const std::vector<std::string>::const_iterator &end
	) const
{
	if (name == end)
	{
		return -1;
	}

	auto tailCallCandidate = entryPoint
		.byName(*name);

	if (!tailCallCandidate)
	{
		return tryCreateTailCall(entryPoint, values, name + 1, end);
	}

	auto targetValues = tailCallCandidate->values();

	if (targetValues.size() != values.size())
	{
		return tryCreateTailCall(entryPoint, values, name + 1, end);
	}

	int8_t min = 0;
	int8_t max = 0;

	std::transform(targetValues.begin(), targetValues.end(), values.begin(), extremities_iterator(min, max), [=](auto storage, auto value)
	{
		auto storageType = storage->type();
		auto valueType = value->type();

		return valueType->compatibility(storageType, entryPoint);
	});

	if (min < 0 || max > 0)
	{
		return std::min(max
			, tryCreateTailCall(entryPoint, values, name + 1, end)
			);
	}

	auto tailCallTarget = findTailCallTarget(tailCallCandidate, values);

	if (!tailCallTarget)
	{
		return tryCreateTailCall(entryPoint, values, name + 1, end);
	}

	auto zipped = zip(values, targetValues);

	auto resultEntryPoint = std::accumulate(zipped.begin(), zipped.end(), entryPoint, [&](auto accumulatedEntryPoint, auto result)
	{
		auto [value, storage] = result;

		return ValueHelper::transferValue(accumulatedEntryPoint, value, storage);
	});

	linkBlocks(resultEntryPoint.block(), tailCallTarget->block());

	return 0;
}

std::vector<DzResult> FunctionCallProxyNode::build(const EntryPoint &entryPoint, Stack values) const
{
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	insertBlock(block, function);

	auto score = tryCreateTailCall(entryPoint, values, begin(m_names), end(m_names));

	if (score == 0)
	{
		return std::vector<DzResult>();
	}

	if (score == 1)
	{
		throw new std::exception(); // TODO
	}

	auto functions = entryPoint.functions();

	for (auto &name : m_names)
	{
		for (auto [i, end] = functions.equal_range(name); i != end; i++)
		{
			auto function = i->second;

			// Naive. Really naive.
			if (function->attribute() == FunctionAttribute::Iterator)
			{
				auto capsule = new CapsuleNode(values, m_regularCall);
				auto generator = new IteratorValueGenerator(new IteratorType(), capsule, entryPoint);
				auto lazy = new LazyValue(generator);

				return {{ entryPoint, lazy }};
			}
		}
	}

	auto junction = new JunctionNode(m_regularCall);

	return junction->build(entryPoint, values);
}
