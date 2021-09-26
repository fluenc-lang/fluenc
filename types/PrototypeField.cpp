#include "PrototypeField.h"

PrototypeField::PrototypeField(const std::string &name, DzValue *defaultValue)
	: m_name(name)
	, m_defaultValue(defaultValue)
{
}

std::string PrototypeField::name() const
{
	return m_name;
}

DzValue *PrototypeField::defaultValue() const
{
	return m_defaultValue;
}
