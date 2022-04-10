#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>

#include "values/ArrayValueGenerator.h"
#include "values/ArrayValue.h"
#include "values/ReferenceValue.h"
#include "IteratorStorage.h"

#include "types/Int64Type.h"
#include "types/ArrayType.h"
#include "types/IteratorType.h"

ArrayValueGenerator::ArrayValueGenerator(size_t id
	, const Node *iterator
	, const std::vector<DzResult> &values
	)
	: m_id(id)
	, m_iterator(iterator)
	, m_values(values)
{
}

const IIteratable *ArrayValueGenerator::generate(const EntryPoint &entryPoint) const
{
	auto iteratorStorage = entryPoint
		.iteratorStorage();

	auto index = iteratorStorage->getOrCreate(m_id, entryPoint);

	return new ArrayValue(index, m_iterator, m_values);
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
