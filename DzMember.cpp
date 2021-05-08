#include "DzMember.h"

DzMember::DzMember(const std::string &name, DzTypeName *type)
	: m_name(name)
	, m_type(type)
{
}

std::string DzMember::name() const
{
	return m_name;
}

DzTypeName *DzMember::type() const
{
	return m_type;
}
