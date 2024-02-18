#include <llvm/IR/Constants.h>

#include "ScalarValue.h"
#include "IRBuilderEx.h"

#include "values/StringIteratableGenerator.h"
#include "values/StringIteratable.h"
#include "values/Iteratable.h"

#include "types/IteratorType.h"
#include "types/Int64Type.h"
#include "types/StringType.h"

StringIteratableGenerator::StringIteratableGenerator(const reference_value *address, const reference_value *length)
	: m_address(address)
	, m_length(length)
{
}

const fragment *StringIteratableGenerator::generate(const EntryPoint &entryPoint, GenerationMode mode) const
{
	UNUSED(mode);

	IRBuilderEx builder(entryPoint);

	auto context = entryPoint.context();

        auto indexType = int64_type::instance();
	auto storageType = indexType->storageType(*context);

        auto zero = new scalar_value(indexType
		, llvm::ConstantInt::get(storageType, 0)
		);

	auto index = entryPoint.alloc(indexType, "index");

	builder.createStore(zero, index);

	auto subject = new StringIteratable(index, m_address, m_length);

	return new Iteratable(subject
                              , string_type::instance()
		);
}

const ILazyValueGenerator *StringIteratableGenerator::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}

const Type *StringIteratableGenerator::type() const
{
    return iterator_type::instance();
}
