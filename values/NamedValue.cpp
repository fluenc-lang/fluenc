#include "NamedValue.h"
#include "EntryPoint.h"

NamedValue::NamedValue(const std::string &name, const EntryPoint &entryPoint, DzValue *subject)
	: m_name(name)
	, m_entryPoint(new EntryPoint(entryPoint))
	, m_subject(subject)
{
}

std::string NamedValue::name() const
{
	return m_name;
}

const EntryPoint *NamedValue::entryPoint() const
{
	return m_entryPoint;
}

DzValue *NamedValue::subject() const
{
	return m_subject;
}
