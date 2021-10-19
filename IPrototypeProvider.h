#ifndef IPROTOTYPEPROVIDER_H
#define IPROTOTYPEPROVIDER_H

#include "DzValue.h"

class IPrototype;

class IPrototypeProvider
{
	public:
		virtual IPrototype *provide(const EntryPoint &entryPoint, Stack &values) = 0;
};

#endif // IPROTOTYPEPROVIDER_H
