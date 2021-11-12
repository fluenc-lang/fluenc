#include "DependentValue.h"

DependentValue::DependentValue(const EntryPoint *provider)
	: m_provider(provider)
{
}

const Type *DependentValue::type() const
{
	throw std::exception();
}

const EntryPoint *DependentValue::provider() const
{
	return m_provider;
}
