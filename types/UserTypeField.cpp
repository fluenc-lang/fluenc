#include "UserTypeField.h"

UserTypeField::UserTypeField(size_t index, const std::string &name, const Type *type)
	: m_index(index)
	, m_name(name)
	, m_type(type)
{
}

size_t UserTypeField::index() const
{
	return m_index;
}

std::string UserTypeField::name() const
{
	return m_name;
}

const Type *UserTypeField::type() const
{
	return m_type;
}
