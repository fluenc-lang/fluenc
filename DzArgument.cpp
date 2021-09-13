#include "DzArgument.h"
#include "DzTypeName.h"
#include "EntryPoint.h"

DzArgument::DzArgument(const std::string &name, DzTypeName *type)
	: m_name(name)
	, m_type(type)
{
}

std::string DzArgument::name() const
{
	return m_name;
}

Type *DzArgument::type(const EntryPoint &entryPoint) const
{
	return m_type->resolve(entryPoint);
}
