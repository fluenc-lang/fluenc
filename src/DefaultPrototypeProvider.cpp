#include "DefaultPrototypeProvider.h"
#include "DzTypeName.h"
#include "Utility.h"

DefaultPrototypeProvider::DefaultPrototypeProvider(ITypeName *type)
	: m_type(type)
{
}

IPrototype *DefaultPrototypeProvider::provide(const EntryPoint &entryPoint, Stack<BaseValue> &values) const
{
	UNUSED(values);

	return (IPrototype *)m_type->resolve(entryPoint);
}
