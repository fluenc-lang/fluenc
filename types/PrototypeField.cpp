#include "PrototypeField.h"

PrototypeField::PrototypeField(const std::string &name, const DzValue *defaultValue)
	: m_name(name)
	, m_defaultValue(defaultValue)
{
}

std::string PrototypeField::name() const
{
	return m_name;
}

const DzValue *PrototypeField::defaultValue() const
{
	return m_defaultValue;
}
