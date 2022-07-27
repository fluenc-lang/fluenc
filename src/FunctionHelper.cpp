#include "FunctionHelper.h"

#include "iterators/ExtremitiesIterator.h"

std::tuple<int8_t, const EntryPoint *, Stack> FunctionHelper::tryCreateTailCall(const EntryPoint &entryPoint
	, const Stack &values
	, const std::vector<std::string>::const_iterator &name
	, const std::vector<std::string>::const_iterator &end
	)
{
	if (name == end)
	{
		return { -1, nullptr, Stack() };
	}

	auto tailCallCandidate = entryPoint
		.byName(*name);

	if (!tailCallCandidate)
	{
		return tryCreateTailCall(entryPoint, values, name + 1, end);
	}

	auto targetEntry = tailCallCandidate->entry();
	auto targetValues = targetEntry->values();

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
		auto [score, forwardedEntryPoint, forwardedValues] = tryCreateTailCall(entryPoint, values, name + 1, end);

		return { std::min(max, score), forwardedEntryPoint, forwardedValues };
	}

	auto tailCallTarget = findTailCallTarget(tailCallCandidate, values);

	if (!tailCallTarget)
	{
		return tryCreateTailCall(entryPoint, values, name + 1, end);
	}

	return { 0, tailCallTarget->entry(), targetValues };
}
