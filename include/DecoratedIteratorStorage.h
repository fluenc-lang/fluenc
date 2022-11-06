#ifndef DECORATEDITERATORSTORAGE_H
#define DECORATEDITERATORSTORAGE_H

#include "IIteratorStorage.h"

class DecoratedIteratorStorage : public IIteratorStorage
{
	public:
		DecoratedIteratorStorage(IIteratorStorage *subject, std::filesystem::path path);

		const ReferenceValue *getOrCreate(std::filesystem::path path, const EntryPoint &entryPoint);

	private:
		IIteratorStorage *m_subject;

		std::filesystem::path m_path;
};

#endif // DECORATEDITERATORSTORAGE_H
