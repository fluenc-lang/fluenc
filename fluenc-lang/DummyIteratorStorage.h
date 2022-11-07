#ifndef DUMMYITERATORSTORAGE_H
#define DUMMYITERATORSTORAGE_H

#include "IIteratorStorage.h"

class DummyIteratorStorage : public IIteratorStorage
{
	public:
		const ReferenceValue *getOrCreate(size_t id, const EntryPoint &entryPoint);
};

#endif // DUMMYITERATORSTORAGE_H
