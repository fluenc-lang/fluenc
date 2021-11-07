#include "ArgumentValue.h"

ArgumentValue::ArgumentValue(int index, TypedValue *subject)
	: m_index(index)
	, m_subject(subject)
{
}

int ArgumentValue::index() const
{
	return m_index;
}

TypedValue *ArgumentValue::subject() const
{
	return m_subject;
}
