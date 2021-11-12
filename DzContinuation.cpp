#include <numeric>

#include <llvm/IR/Instructions.h>

#include "DzContinuation.h"
#include "Type.h"
#include "IndexIterator.h"

#include "values/TypedValue.h"
#include "values/DependentValue.h"

std::vector<DzResult> DzContinuation::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	auto numberOfArguments = values.size();
	auto tailCallValues = entryPoint.values();

	auto tailCallTarget = std::accumulate(index_iterator(0u), index_iterator(numberOfArguments), &entryPoint, [&](const EntryPoint *target, size_t)
	{
		auto value = values.pop();
		auto storage = tailCallValues.pop();

		if (auto computedValue = dynamic_cast<const TypedValue *>(value))
		{
			auto valueType = storage->type();
			auto valueStorageType = valueType->storageType(*context);

			auto valueAlign = dataLayout.getABITypeAlign(valueStorageType);

			auto store = new llvm::StoreInst(*computedValue, *static_cast<const TypedValue *>(storage), false, valueAlign, block);

			UNUSED(store);
		}
		else if (auto dependentValue = dynamic_cast<const DependentValue *>(value))
		{
			auto provider = dependentValue->provider();

			if (provider->depth() < target->depth())
			{
				return provider;
			}
		}

		return target;
	});

	return {{ *tailCallTarget, Stack() }};
}
