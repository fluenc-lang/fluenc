#ifndef DEFAULTPROTOTYPEPROVIDER_H
#define DEFAULTPROTOTYPEPROVIDER_H

#include "IPrototypeProvider.h"

class ITypeName;

class DefaultPrototypeProvider : public IPrototypeProvider
{
	public:
		DefaultPrototypeProvider(ITypeName *type);

		IPrototype *provide(const EntryPoint &entryPoint, Stack &values) override;

	private:
		ITypeName *m_type;
};

#endif // DEFAULTPROTOTYPEPROVIDER_H
