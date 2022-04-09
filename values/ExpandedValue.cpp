#include "ExpandedValue.h"

#include "types/IteratorType.h"

ExpandedValue::ExpandedValue(const Type *iteratorType, const EntryPoint *provider)
	: m_iteratorType(iteratorType)
	, m_provider(provider)
{
}

const Type *ExpandedValue::type() const
{
	return m_iteratorType;
}

const BaseValue *ExpandedValue::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}

const EntryPoint *ExpandedValue::provider() const
{
	return m_provider;
}
