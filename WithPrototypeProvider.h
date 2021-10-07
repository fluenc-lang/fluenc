#ifndef WITHPROTOTYPEPROVIDER_H
#define WITHPROTOTYPEPROVIDER_H

#include "IPrototypeProvider.h"

class WithPrototypeProvider : public IPrototypeProvider
{
	public:
		static WithPrototypeProvider *instance();

		IPrototype *provide(const EntryPoint &entryPoint, Stack values) override;
};

#endif // WITHPROTOTYPEPROVIDER_H
