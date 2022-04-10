#include "types/PrototypeFieldEmbryo.h"
#include "DzTypeName.h"

PrototypeFieldEmbryo::PrototypeFieldEmbryo(const std::string &name
	, const Node *defaultValue
	, const ITypeName *type
	)
	: m_name(name)
	, m_defaultValue(defaultValue)
	, m_type(type)
{
}

std::string PrototypeFieldEmbryo::name() const
{
	return m_name;
}

const Node *PrototypeFieldEmbryo::defaultValue() const
{
	return m_defaultValue;
}

const Type *PrototypeFieldEmbryo::type(const EntryPoint &entryPoint) const
{
	if (m_type)
	{
		return m_type->resolve(entryPoint);
	}

	return nullptr;
}
