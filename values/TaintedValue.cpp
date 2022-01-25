#include "TaintedValue.h"

TaintedValue::TaintedValue(const BaseValue *subject)
	: m_subject(subject)
{
}

const Type *TaintedValue::type() const
{
	return m_subject->type();
}

const BaseValue *TaintedValue::clone(const EntryPoint &entryPoint) const
{
	return new TaintedValue(m_subject->clone(entryPoint));
}

const BaseValue *TaintedValue::subject() const
{
	return m_subject;
}
