#include "UserTypeField.h"

UserTypeField::UserTypeField(int index, const std::string &name, Type *type)
	: m_index(index)
	, m_name(name)
	, m_type(type)
{
}

int UserTypeField::index() const
{
	return m_index;
}

std::string UserTypeField::name() const
{
	return m_name;
}

Type *UserTypeField::type() const
{
	return m_type;
}
