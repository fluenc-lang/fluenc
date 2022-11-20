#include "values/ForwardedValue.h"

ForwardedValue::ForwardedValue(const BaseValue *subject)
	: m_subject(subject)
{
}

const BaseValue *ForwardedValue::subject() const
{
	return m_subject;
}

const Type *ForwardedValue::type() const
{
	return m_subject->type();
}

const BaseValue *ForwardedValue::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	auto subject = m_subject->clone(entryPoint, strategy);

	return new ForwardedValue(subject);
}

const ValueMetadata &ForwardedValue::metadata() const
{
	return m_subject->metadata();
}
