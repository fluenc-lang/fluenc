#include "DependentValue.h"

#include "types/IteratorType.h"

DependentValue::DependentValue(const EntryPoint *provider)
	: m_provider(provider)
{
}

const Type *DependentValue::type() const
{
	return IteratorType::instance();
}

const BaseValue *DependentValue::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}

const EntryPoint *DependentValue::provider() const
{
	return m_provider;
}
