#include "IteratorStorage.h"

#include "values/StringIteratableGenerator.h"
#include "values/StringIteratable.h"

#include "types/IteratorType.h"

StringIteratableGenerator::StringIteratableGenerator(llvm::Value *address, size_t id, size_t length)
	: m_address(address)
	, m_id(id)
	, m_length(length)
{
}

const IIteratable *StringIteratableGenerator::generate(const EntryPoint &entryPoint) const
{
	auto iteratorStorage = entryPoint
		.iteratorStorage();

	auto index = iteratorStorage->getOrCreate(m_id, entryPoint);

	return new StringIteratable(index, m_address, m_length);
}

const ILazyValueGenerator *StringIteratableGenerator::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}

const ILazyValueGenerator *StringIteratableGenerator::forward(size_t id) const
{
	return new StringIteratableGenerator(m_address, id, m_length);
}

const Type *StringIteratableGenerator::type() const
{
	return IteratorType::instance();
}
