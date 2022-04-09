#include <numeric>

#include "ArrayValueGenerator.h"
#include "ArrayValue.h"
#include "ReferenceValue.h"
#include "IteratorStorage.h"
#include "IndexIterator.h"

#include "nodes/ArrayElementNode.h"

#include "types/Int64Type.h"
#include "types/ArrayType.h"
#include "types/IteratorType.h"

ArrayValueGenerator::ArrayValueGenerator(const std::vector<DzResult> &values, size_t id, size_t size)
	: m_values(values)
	, m_id(id)
	, m_size(size)
{
}

const IIteratable *ArrayValueGenerator::generate(const EntryPoint &entryPoint) const
{
	auto iteratorStorage = entryPoint
		.iteratorStorage();

	auto index = iteratorStorage->getOrCreate(m_id, entryPoint);

	auto iterator = std::accumulate(index_iterator(0u), index_iterator(m_size), (Node *)nullptr, [&](auto next, auto)
	{
		return new ArrayElementNode(type(), next);
	});

	return new ArrayValue(index, iterator, m_values);
}

const Type *ArrayValueGenerator::type() const
{
	if (m_values.size() == 1)
	{
		auto [_, values] = *m_values.begin();

		std::vector<const Type *> types;

		std::transform(values.rbegin(), values.rend(), std::back_inserter(types), [](auto value)
		{
			return value->type();
		});

		return ArrayType::get(types);
	}

	return IteratorType::instance();
}

const ILazyValueGenerator *ArrayValueGenerator::clone(const EntryPoint &entryPoint) const
{
	if (m_values.size() == 1)
	{
		auto [inputEntryPoint, inputValues] = m_values[0];

		std::vector<const BaseValue *> clonedValues;

		std::transform(inputValues.begin(), inputValues.end(), std::back_inserter(clonedValues), [&](auto value)
		{
			return value->clone(entryPoint);
		});

		return new ArrayValueGenerator({{ inputEntryPoint, clonedValues }}, m_id, m_size);
	}

	throw new std::exception();
}
