#include "values/PlaceholderValue.h"
#include "Utility.h"

#include "types/PlaceholderType.h"

PlaceholderValue *PlaceholderValue::instance()
{
	static PlaceholderValue instance;

	return &instance;
}

const Type *PlaceholderValue::type() const
{
	static PlaceholderType type;

	return &type;
}

const BaseValue *PlaceholderValue::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}
