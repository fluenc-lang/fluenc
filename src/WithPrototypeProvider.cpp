#include "WithPrototypeProvider.h"

#include "types/WithPrototype.h"

#include "values/ScalarValue.h"
#include "values/UserTypeValue.h"

WithPrototypeProvider *WithPrototypeProvider::instance()
{
	static WithPrototypeProvider instance;

	return &instance;
}

IPrototype *WithPrototypeProvider::provide(const EntryPoint &entryPoint, Stack &values)
{
	UNUSED(entryPoint);

	auto addressOfValue = values.require<UserTypeValue>();

	return new WithPrototype(addressOfValue);
}
