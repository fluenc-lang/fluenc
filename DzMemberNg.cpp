#include "DzMemberNg.h"

DzMemberNg::DzMemberNg(const std::string &name, DzTypeNameNg *type)
	: m_name(name)
	, m_type(type)
{
}

std::string DzMemberNg::name() const
{
	return m_name;
}

DzTypeNameNg *DzMemberNg::type() const
{
	return m_type;
}
