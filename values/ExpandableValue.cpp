#include "ExpandableValue.h"
#include "EntryPoint.h"

#include "types/IteratorType.h"

ExpandableValue::ExpandableValue(const EntryPoint &provider, const DzValue *chain)
	: m_provider(new EntryPoint(provider))
	, m_chain(chain)
{
}

const Type *ExpandableValue::type() const
{
	return IteratorType::instance();
}

const BaseValue *ExpandableValue::clone(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return this;
}

const EntryPoint *ExpandableValue::provider() const
{
	return m_provider;
}

const DzValue *ExpandableValue::chain() const
{
	return m_chain;
}
