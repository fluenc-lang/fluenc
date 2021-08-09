#include "DzArgument.h"

DzArgument::DzArgument(const std::string &name, DzTypeName *type)
	: m_name(name)
	, m_type(type)
{
}

std::string DzArgument::name() const
{
	return m_name;
}

DzTypeName *DzArgument::type() const
{
	return m_type;
}
