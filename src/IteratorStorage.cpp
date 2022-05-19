#include <llvm/IR/Instructions.h>

#include "IteratorStorage.h"
#include "EntryPoint.h"
#include "IRBuilderEx.h"

#include "values/ScalarValue.h"

#include "types/Int64Type.h"

const ReferenceValue *IteratorStorage::getOrCreate(size_t id, const EntryPoint &entryPoint)
{
	auto indexType = Int64Type::instance();

	auto iterator = m_storage.find(id);

	if (iterator == m_storage.end())
	{
		IRBuilderEx builder(entryPoint);

		auto &context = entryPoint.context();

		auto storageType = indexType->storageType(*context);

		auto zero = new ScalarValue(indexType
			, llvm::ConstantInt::get(storageType, 0)
			);

		std::ostringstream name;
		name << "index_";
		name << id;

		auto alloc = entryPoint.alloc(indexType, name.str());

		builder.createStore(zero, alloc);

		m_storage.insert({ id, alloc });

		return alloc;
	}

	return iterator->second;
}
