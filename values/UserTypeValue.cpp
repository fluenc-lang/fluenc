#include "UserTypeValue.h"

#include "types/UserType.h"

UserTypeValue::UserTypeValue(const Type *type, const std::vector<const NamedValue *> &values)
	: m_type(type)
	, m_values(values)
{
}

const Type *UserTypeValue::type() const
{
	return m_type;
}

std::vector<const NamedValue *> UserTypeValue::fields() const
{
	return m_values;
}
