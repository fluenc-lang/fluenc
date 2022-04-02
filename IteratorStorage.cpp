#include <llvm/IR/Instructions.h>

#include "IteratorStorage.h"
#include "EntryPoint.h"

#include "types/Int64Type.h"

llvm::Value *IteratorStorage::getOrCreate(size_t id, const EntryPoint &entryPoint)
{
	auto indexType = Int64Type::instance();

	auto iterator = m_storage.find(id);

	if (iterator == m_storage.end())
	{
		auto block = entryPoint.block();

		auto &context = entryPoint.context();
		auto &module = entryPoint.module();

		auto dataLayout = module->getDataLayout();

		auto storageType = indexType->storageType(*context);

		auto align = dataLayout.getABITypeAlign(storageType);

		auto zero = llvm::ConstantInt::get(storageType, 0);

		auto alloc = entryPoint.alloc(storageType);

		auto store = new llvm::StoreInst(zero, alloc, false, align, block);

		UNUSED(store);

		m_storage.insert({ id, alloc });

		return alloc;
	}

	return iterator->second;
}
