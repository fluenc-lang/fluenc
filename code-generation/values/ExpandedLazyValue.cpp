#include "ExpandedLazyValue.h"
#include "LazyValue.h"

ExpandedLazyValue::ExpandedLazyValue(const lazy_value *subject)
	: m_subject(subject)
{
}

value_id ExpandedLazyValue::id() const
{
    return value_id::expanded_lazy;
}

const Type *ExpandedLazyValue::type() const
{
	return m_subject->type();
}

const BaseValue *ExpandedLazyValue::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
	auto cloned = m_subject->clone(entryPoint, strategy);
        auto casted = reinterpret_cast<const lazy_value *>(cloned);

	return new ExpandedLazyValue(casted);
}

const lazy_value *ExpandedLazyValue::subject() const
{
	return m_subject;
}
