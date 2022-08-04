#ifndef ITERATORSTORAGE_H
#define ITERATORSTORAGE_H

#include <unordered_map>

#include "IIteratorStorage.h"

class IteratorStorage : public IIteratorStorage
{
	public:
		const ReferenceValue *getOrCreate(size_t id, const EntryPoint &entryPoint);

	private:
		std::unordered_map<size_t, const ReferenceValue *> m_storage;
};

#endif // ITERATORSTORAGE_H
