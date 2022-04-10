#include "types/PrototypeField.h"

PrototypeField::PrototypeField(const std::string &name
	, const BaseValue *defaultValue
	, const Type *type
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

const BaseValue *PrototypeField::defaultValue() const
{
	return m_defaultValue;
}

const Type *PrototypeField::type() const
{
	return m_type;
}
