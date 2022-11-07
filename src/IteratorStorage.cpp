#include <sstream>

#include <llvm/IR/Instructions.h>

#include "IteratorStorage.h"
#include "EntryPoint.h"
#include "IRBuilderEx.h"

#include "values/ScalarValue.h"

#include "types/Int64Type.h"

const ReferenceValue *IteratorStorage::getOrCreate(std::filesystem::path path, const EntryPoint &entryPoint)
{
	auto indexType = Int64Type::instance();

	auto iterator = m_storage.find(path.string());

	if (iterator == m_storage.end())
	{
		IRBuilderEx builder(entryPoint);

		auto context = entryPoint.context();

		auto storageType = indexType->storageType(*context);

		auto zero = new ScalarValue(indexType
			, llvm::ConstantInt::get(storageType, 0)
			);

		std::ostringstream name;
		name << "index";

		for (auto &segment : path)
		{
			name << "_" << segment;
		}

		auto alloc = entryPoint.alloc(indexType, name.str());

		builder.createStore(zero, alloc);

		m_storage.insert({ path, alloc });

		return alloc;
	}

	return iterator->second;
}
