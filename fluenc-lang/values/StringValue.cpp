#include "values/StringValue.h"
#include "values/ReferenceValue.h"
#include "values/LazyValue.h"
#include "values/StringIteratableGenerator.h"

#include "types/StringType.h"

StringValue::StringValue(const ReferenceValue *address, const ReferenceValue *length)
	: m_address(address)
	, m_length(length)
{
}

ValueId StringValue::id() const
{
	return ValueId::String;
}

const ReferenceValue *StringValue::reference() const
{
	return m_address;
}

const ReferenceValue *StringValue::length() const
{
	return m_length;
}

const LazyValue *StringValue::iterator(const EntryPoint &entryPoint) const
{
	auto generator = new StringIteratableGenerator(m_address, m_length);

	return new LazyValue(generator, entryPoint);
}

const Type *StringValue::type() const
{
	return StringType::instance();
}

const BaseValue *StringValue::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	auto subject = (ReferenceValue *)m_address->clone(entryPoint, strategy);

	return new StringValue(subject, m_length);
}
