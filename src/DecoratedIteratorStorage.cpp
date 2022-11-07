#include "DecoratedIteratorStorage.h"

DecoratedIteratorStorage::DecoratedIteratorStorage(IIteratorStorage *subject, std::filesystem::path path)
	: m_subject(subject)
	, m_path(path)
{
}

const ReferenceValue *DecoratedIteratorStorage::getOrCreate(std::filesystem::path path, const EntryPoint &entryPoint)
{
	return m_subject->getOrCreate(m_path / path, entryPoint);
}
