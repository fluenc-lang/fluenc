#include <llvm/IR/Constants.h>

#include "ScalarValue.h"
#include "IRBuilderEx.h"

#include "values/StringIteratableGenerator.h"
#include "values/StringIteratable.h"
#include "values/Iteratable.h"

#include "types/IteratorType.h"
#include "types/Int64Type.h"
#include "types/StringType.h"

StringIteratableGenerator::StringIteratableGenerator(const Node *node, llvm::Value *address, size_t id, size_t length)
	: m_node(node)
	, m_address(address)
	, m_id(id)
	, m_length(length)
{
}

const IIteratable *StringIteratableGenerator::generate(const EntryPoint &entryPoint, GenerationMode mode) const
{
	UNUSED(mode);

	IRBuilderEx builder(entryPoint);

	auto context = entryPoint.context();

	auto indexType = Int64Type::instance();
	auto storageType = indexType->storageType(*context);

	auto zero = new ScalarValue(indexType
		, llvm::ConstantInt::get(storageType, 0)
		);

	auto index = entryPoint.alloc(indexType, "index");

	builder.createStore(zero, index);

	auto subject = new StringIteratable(index, m_node, m_address, m_length);

	return new Iteratable(subject
		, StringType::get(m_length)
		);
}

const ILazyValueGenerator *StringIteratableGenerator::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}

const ILazyValueGenerator *StringIteratableGenerator::forward(size_t id) const
{
	return new StringIteratableGenerator(m_node, m_address, id, m_length);
}

const Type *StringIteratableGenerator::type() const
{
	return IteratorType::instance();
}
