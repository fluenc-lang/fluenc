#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>

#include "ArrayValueGenerator.h"
#include "ArrayValue.h"
#include "TypedValue.h"

#include "types/Int64Type.h"

ArrayValueGenerator::ArrayValueGenerator(const DzValue *iterator
	, const std::vector<DzResult> &values
	)
	: m_iterator(iterator)
	, m_values(values)
{
}

IIteratable *ArrayValueGenerator::generate(const EntryPoint &entryPoint) const
{
	auto block = entryPoint.block();

	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto dataLayout = module->getDataLayout();

	auto indexType = Int64Type::instance();
	auto storageType = indexType->storageType(*context);

	auto align = dataLayout.getABITypeAlign(storageType);

	auto zero = llvm::ConstantInt::get(storageType, 0);

	auto alloc = entryPoint.alloc(storageType);

	auto store = new llvm::StoreInst(zero, alloc, false, align, block);

	UNUSED(store);

	auto index = new TypedValue(indexType, alloc);

	return new ArrayValue(index, m_iterator, m_values);
}
