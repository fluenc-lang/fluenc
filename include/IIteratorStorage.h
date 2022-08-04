#ifndef IITERATORSTORAGE_H
#define IITERATORSTORAGE_H

#include <stddef.h>

class EntryPoint;
class ReferenceValue;

class IIteratorStorage
{
	public:
		virtual const ReferenceValue *getOrCreate(size_t id, const EntryPoint &entryPoint) = 0;
};

#endif // IITERATORSTORAGE_H
