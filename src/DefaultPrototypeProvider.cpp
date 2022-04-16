#include "DefaultPrototypeProvider.h"
#include "DzTypeName.h"

DefaultPrototypeProvider::DefaultPrototypeProvider(ITypeName *type)
	: m_type(type)
{
}

IPrototype *DefaultPrototypeProvider::provide(const EntryPoint &entryPoint, Stack &values) const
{
	UNUSED(values);

	return (IPrototype *)m_type->resolve(entryPoint);
}
