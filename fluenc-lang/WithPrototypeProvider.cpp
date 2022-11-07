#include "WithPrototypeProvider.h"
#include "Utility.h"

#include "types/WithPrototype.h"

#include "values/ScalarValue.h"
#include "values/UserTypeValue.h"

WithPrototypeProvider *WithPrototypeProvider::instance()
{
	static WithPrototypeProvider instance;

	return &instance;
}

IPrototype *WithPrototypeProvider::provide(const EntryPoint &entryPoint, Stack &values) const
{
	UNUSED(entryPoint);

	auto addressOfValue = values.require<UserTypeValue>(nullptr);

	return new WithPrototype(addressOfValue);
}
