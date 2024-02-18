#include "IndexedValue.h"

indexed_value::indexed_value(size_t index, const BaseValue *subject)
	: m_index(index)
	, m_subject(subject)
{
}

value_id indexed_value::id() const
{
    return value_id::indexed;
}

size_t indexed_value::index() const
{
	return m_index;
}

const Type *indexed_value::type() const
{
	return m_subject->type();
}

const BaseValue *indexed_value::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
	return new indexed_value(m_index
		, m_subject->clone(entryPoint, strategy)
		);
}

const BaseValue *indexed_value::subject() const
{
	return m_subject;
}
