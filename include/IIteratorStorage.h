#ifndef IITERATORSTORAGE_H
#define IITERATORSTORAGE_H

#include <filesystem>

class EntryPoint;
class ReferenceValue;

class IIteratorStorage
{
	public:
		virtual const ReferenceValue *getOrCreate(std::filesystem::path path, const EntryPoint &entryPoint) = 0;
};

#endif // IITERATORSTORAGE_H
