#include "values/IndexedValue.h"

IndexedValue::IndexedValue(size_t index, const BaseValue *subject)
	: m_index(index)
	, m_subject(subject)
{
}

ValueId IndexedValue::id() const
{
	return ValueId::Indexed;
}

size_t IndexedValue::index() const
{
	return m_index;
}

const Type *IndexedValue::type() const
{
	return m_subject->type();
}

const BaseValue *IndexedValue::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	return new IndexedValue(m_index
		, m_subject->clone(entryPoint, strategy)
		);
}

const BaseValue *IndexedValue::subject() const
{
	return m_subject;
}
