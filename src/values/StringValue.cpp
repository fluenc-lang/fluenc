#include "values/StringValue.h"
#include "values/ScalarValue.h"
#include "values/LazyValue.h"
#include "values/StringIteratableGenerator.h"

#include "types/StringType.h"

StringValue::StringValue(llvm::Value *address, size_t id, size_t length)
	: m_address(address)
	, m_id(id)
	, m_length(length)
{
}

const ScalarValue *StringValue::scalar() const
{
	return new ScalarValue(StringType::instance(), m_address);
}

const LazyValue *StringValue::iterator() const
{
	auto generator = new StringIteratableGenerator(m_address, m_id, m_length);

	return new LazyValue(generator);
}

const Type *StringValue::type() const
{
	return StringType::instance();
}

const BaseValue *StringValue::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}
