#ifndef IPROTOTYPEPROVIDER_H
#define IPROTOTYPEPROVIDER_H

#include "DzValue.h"

class Prototype;

class IPrototypeProvider
{
	public:
		virtual Prototype *provide(const EntryPoint &entryPoint, Stack values) = 0;
};

#endif // IPROTOTYPEPROVIDER_H
