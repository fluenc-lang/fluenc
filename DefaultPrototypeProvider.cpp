#include "DefaultPrototypeProvider.h"
#include "DzTypeName.h"

DefaultPrototypeProvider::DefaultPrototypeProvider(DzTypeName *type)
	: m_type(type)
{
}

IPrototype *DefaultPrototypeProvider::provide(const EntryPoint &entryPoint, Stack &values)
{
	UNUSED(values);

	return (IPrototype *)m_type->resolve(entryPoint);
}
