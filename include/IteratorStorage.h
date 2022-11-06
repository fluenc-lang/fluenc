#ifndef ITERATORSTORAGE_H
#define ITERATORSTORAGE_H

#include <unordered_map>
#include <filesystem>

#include "IIteratorStorage.h"

class IteratorStorage : public IIteratorStorage
{
	public:
		const ReferenceValue *getOrCreate(std::filesystem::path path, const EntryPoint &entryPoint);

	private:
		std::unordered_map<std::filesystem::path, const ReferenceValue *> m_storage;
};

#endif // ITERATORSTORAGE_H
