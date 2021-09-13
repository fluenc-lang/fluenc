#include <llvm/IR/IRBuilder.h>

#include "DzInstantiation.h"
#include "DzTypeName.h"

DzInstantiation::DzInstantiation(DzValue *consumer
	, DzTypeName *type
	, const std::vector<std::string> &fields
	)
	: m_consumer(consumer)
	, m_type(type)
	, m_fields(fields)
{
}

std::vector<DzResult> DzInstantiation::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	std::vector<llvm::Type *> types;

	std::transform(begin(m_fields), end(m_fields), values.begin(), std::back_insert_iterator(types), [&](std::string, TypedValue value)
	{
		auto type = value.type();

		return type->storageType(*context);
	});

	auto block = entryPoint.block();

	auto prototype = (Prototype *)m_type->resolve(entryPoint);

	auto dataLayout = module->getDataLayout();

	auto st = llvm::StructType::get(*context, types);

	auto addressSpace = dataLayout.getAllocaAddrSpace();
	auto align = dataLayout.getABITypeAlign(st);

	auto alloc = new llvm::AllocaInst(st, addressSpace, nullptr, align, "instance", block);

	int i = 0;

	for (auto &k : m_fields)
	{
		llvm::Value *indexes[] =
		{
			llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0),
			llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), i++)
		};

		auto o = llvm::GetElementPtrInst::CreateInBounds(alloc, indexes, k, block);

		auto value = values.pop();

		auto valueType = value.type();
		auto valueStorageType = valueType->storageType(*context);

		auto valueAlign = dataLayout.getABITypeAlign(valueStorageType);

		auto store = new llvm::StoreInst(value, o, false, valueAlign, block);

		UNUSED(store);
	}

	auto load = new llvm::LoadInst(st, alloc, "by_value", false, align, block);

	auto type = new UserType(prototype, st);

	values.push(TypedValue(type, load));

	return m_consumer->build(entryPoint, values);
}
