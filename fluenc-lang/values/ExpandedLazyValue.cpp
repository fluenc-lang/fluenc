#include "ExpandedLazyValue.h"
#include "LazyValue.h"

ExpandedLazyValue::ExpandedLazyValue(const LazyValue *subject)
	: m_subject(subject)
{
}

ValueId ExpandedLazyValue::id() const
{
	return ValueId::ExpandedLazy;
}

const Type *ExpandedLazyValue::type() const
{
	return m_subject->type();
}

const BaseValue *ExpandedLazyValue::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	auto cloned = m_subject->clone(entryPoint, strategy);
	auto casted = reinterpret_cast<const LazyValue *>(cloned);

	return new ExpandedLazyValue(casted);
}

const LazyValue *ExpandedLazyValue::subject() const
{
	return m_subject;
}
