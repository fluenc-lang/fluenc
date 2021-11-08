#include "DependentValue.h"

DependentValue::DependentValue(const Type *type, const EntryPoint *provider, const DzValue *chain)
	: m_type(type)
	, m_provider(provider)
	, m_chain(chain)
{
}

const Type *DependentValue::type() const
{
	return m_type;
}

const EntryPoint *DependentValue::provider() const
{
	return m_provider;
}

const DzValue *DependentValue::chain() const
{
	return m_chain;
}
