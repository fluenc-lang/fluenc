#ifndef DEFAULTPROTOTYPEPROVIDER_H
#define DEFAULTPROTOTYPEPROVIDER_H

#include "IPrototypeProvider.h"

class DzTypeName;

class DefaultPrototypeProvider : public IPrototypeProvider
{
	public:
		DefaultPrototypeProvider(DzTypeName *type);

		IPrototype *provide(const EntryPoint &entryPoint, Stack values) override;

	private:
		DzTypeName *m_type;
};

#endif // DEFAULTPROTOTYPEPROVIDER_H
