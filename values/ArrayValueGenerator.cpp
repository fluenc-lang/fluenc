#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>

#include "ArrayValueGenerator.h"
#include "ArrayValue.h"
#include "TypedValue.h"
#include "IteratorStorage.h"

#include "types/Int64Type.h"

ArrayValueGenerator::ArrayValueGenerator(size_t id
	, const DzValue *iterator
	, const std::vector<DzResult> &values
	)
	: m_id(id)
	, m_iterator(iterator)
	, m_values(values)
{
}

IIteratable *ArrayValueGenerator::generate(const EntryPoint &entryPoint) const
{
	auto iteratorStorage = entryPoint
		.iteratorStorage();

	auto index = new TypedValue(Int64Type::instance()
		, iteratorStorage->getOrCreate(m_id, entryPoint)
		);

	return new ArrayValue(index, m_iterator, m_values);
}
