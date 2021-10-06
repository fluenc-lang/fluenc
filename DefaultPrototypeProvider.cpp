#include "DefaultPrototypeProvider.h"
#include "DzTypeName.h"

DefaultPrototypeProvider::DefaultPrototypeProvider(DzTypeName *type)
	: m_type(type)
{
}

Prototype *DefaultPrototypeProvider::provide(const EntryPoint &entryPoint, Stack values)
{
	UNUSED(values);

	return (Prototype *)m_type->resolve(entryPoint);
}
