#include "UserTypeField.h"

UserTypeField::UserTypeField(const std::string &name, Type *type)
	: m_name(name)
	, m_type(type)
{
}

std::string UserTypeField::name() const
{
	return m_name;
}

Type *UserTypeField::type() const
{
	return m_type;
}
