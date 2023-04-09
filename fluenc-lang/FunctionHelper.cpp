#include "FunctionHelper.h"
#include "Type.h"
#include "TypeCompatibilityCalculator.h"

#include "iterators/ExtremitiesIterator.h"

#include <iostream>

std::tuple<int8_t, const EntryPoint *> findEntryPoint(const EntryPoint &entryPoint, const std::string &name, const Stack &values)
{
	auto tailCallCandidate = entryPoint
		.byName(name);

	if (!tailCallCandidate)
	{
		return { -1, nullptr };
	}

	auto targetEntry = tailCallCandidate->entry();
	auto targetValues = targetEntry->values();

	if (targetValues.size() != values.size())
	{
		return findEntryPoint(*targetEntry->parent(), name, values);
	}

	int8_t min = 0;
	int8_t max = 0;

	std::cout << "Trying to match with:" << std::endl;

	std::transform(targetValues.begin(), targetValues.end(), values.begin(), extremities_iterator(min, max), [=](auto storage, auto value)
	{
		auto storageType = storage->type();
		auto valueType = value->type();

		std::cout << storageType->fullName() << std::endl;

		return TypeCompatibilityCalculator::calculate(entryPoint, valueType, storageType);
	});

	std::cout << std::endl;

	if (min < 0 || max > 0)
	{
		auto [score, result] = findEntryPoint(*targetEntry->parent(), name, values);

		return { std::min(max, score), result };
	}

	return { 0, tailCallCandidate };
}

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

	auto [score, tailCallCandidate] = findEntryPoint(entryPoint, *name, values);

	if (!tailCallCandidate)
	{
		return tryCreateTailCall(entryPoint, values, name + 1, end);
	}

	auto targetEntry = tailCallCandidate->entry();
	auto targetValues = targetEntry->values();

	auto tailCallTarget = findTailCallTarget(tailCallCandidate, values);

	if (!tailCallTarget)
	{
		return tryCreateTailCall(entryPoint, values, name + 1, end);
	}

	return { score, tailCallTarget->entry(), targetValues };
}
