#include <llvm/IR/Instructions.h>

#include "DzContinuation.h"
#include "Type.h"

#include "values/TypedValue.h"

std::vector<DzResult> DzContinuation::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	auto numberOfArguments = values.size();
	auto tailCallValues = entryPoint.values();

	for (auto i = 0ul; i < numberOfArguments; i++)
	{
		auto value = values.require<TypedValue>();
		auto storage = tailCallValues.require<TypedValue>();

		auto valueType = storage->type();
		auto valueStorageType = valueType->storageType(*context);

		auto valueAlign = dataLayout.getABITypeAlign(valueStorageType);

		auto store = new llvm::StoreInst(*value, *storage, false, valueAlign, block);

		UNUSED(store);
	}

	return std::vector<DzResult>();
}
