#include "DummyIteratorStorage.h"
#include "Utility.h"

const ReferenceValue *DummyIteratorStorage::getOrCreate(size_t id, const EntryPoint &entryPoint)
{
	UNUSED(id);
	UNUSED(entryPoint);

	return nullptr;
}
