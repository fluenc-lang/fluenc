#include "WithPrototypeProvider.h"
#include "DzFieldAccess.h"

#include "types/UserType.h"
#include "types/WithPrototype.h"

WithPrototypeProvider *WithPrototypeProvider::instance()
{
	static WithPrototypeProvider instance;

	return &instance;
}

IPrototype *WithPrototypeProvider::provide(const EntryPoint &entryPoint, Stack &values)
{
	UNUSED(entryPoint);

	auto addressOfValue = values.pop();

	return new WithPrototype(addressOfValue);
}
