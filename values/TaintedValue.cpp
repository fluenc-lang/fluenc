#include "TaintedValue.h"

TaintedValue::TaintedValue(const BaseValue *subject)
	: m_subject(subject)
{
}

const Type *TaintedValue::type() const
{
	return m_subject->type();
}

const BaseValue *TaintedValue::subject() const
{
	return m_subject;
}
