#include "PrototypeField.h"

PrototypeField::PrototypeField(const std::string &name
	, const DzValue *defaultValue
	, const DzTypeName *type
	)
	: m_name(name)
	, m_defaultValue(defaultValue)
	, m_type(type)
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

const DzTypeName *PrototypeField::type() const
{
	return m_type;
}
