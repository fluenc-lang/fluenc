#include "values/StringValue.h"
#include "values/ReferenceValue.h"
#include "values/LazyValue.h"
#include "values/StringIteratableGenerator.h"

#include "types/StringType.h"

StringValue::StringValue(const reference_value *address, const reference_value *length)
	: m_address(address)
	, m_length(length)
{
}

value_id StringValue::id() const
{
    return value_id::string;
}

const reference_value *StringValue::reference() const
{
	return m_address;
}

const reference_value *StringValue::length() const
{
	return m_length;
}

const lazy_value *StringValue::iterator(const EntryPoint &entryPoint) const
{
	auto generator = new StringIteratableGenerator(m_address, m_length);

        return new lazy_value(generator, entryPoint);
}

const Type *StringValue::type() const
{
    return string_type::instance();
}

const BaseValue *StringValue::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
    auto subject = (reference_value *)m_address->clone(entryPoint, strategy);

	return new StringValue(subject, m_length);
}
