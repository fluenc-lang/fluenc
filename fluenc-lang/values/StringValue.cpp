#include "values/StringValue.h"
#include "values/ReferenceValue.h"
#include "values/LazyValue.h"
#include "values/StringIteratableGenerator.h"

#include "types/StringType.h"

StringValue::StringValue(const ReferenceValue *address, size_t id, size_t length)
	: m_address(address)
	, m_id(id)
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

const LazyValue *StringValue::iterator() const
{
	auto generator = new StringIteratableGenerator(*m_address, m_id, m_length);

	return new LazyValue(generator, EntryPoint());
}

const Type *StringValue::type() const
{
	return StringType::get(m_length);
}

const BaseValue *StringValue::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	auto subject = (ReferenceValue *)m_address->clone(entryPoint, strategy);

	return new StringValue(subject, m_id, m_length);
}

const BaseValue *StringValue::forward(size_t id) const
{
	return new StringValue(m_address, id, m_length);
}
