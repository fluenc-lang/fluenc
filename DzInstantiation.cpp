#include <llvm/IR/IRBuilder.h>

#include "DzInstantiation.h"
#include "DzTypeName.h"
#include "IndexIterator.h"

DzInstantiation::DzInstantiation(DzValue *consumer
	, DzTypeName *type
	, const std::vector<std::string> &fields
	)
	: m_consumer(consumer)
	, m_type(type)
	, m_fields(begin(fields), end(fields))
{
}

std::vector<DzResult> DzInstantiation::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto prototype = (Prototype *)m_type->resolve(entryPoint);

	auto prototypeFields = prototype->fields();

	std::vector<FieldEmbryo> fieldEmbryos;

	std::transform(begin(prototypeFields), end(prototypeFields), index_iterator(), std::back_insert_iterator(fieldEmbryos), [&](auto field, auto index) -> FieldEmbryo
	{
		if (m_fields.contains(field.name()))
		{
			return { index, field, values.pop() };
		}

		auto defaultValue = field.defaultValue();

		if (!defaultValue)
		{
			throw new std::exception();
		}

		for (auto &[_, defaultValues] : defaultValue->build(entryPoint, Stack()))
		{
			return { index, field, *defaultValues.begin() };
		}

		throw new std::exception();
	});

	std::vector<llvm::Type *> types;

	std::transform(begin(fieldEmbryos), end(fieldEmbryos), std::back_insert_iterator(types), [&](auto embryo)
	{
		return embryo.value.type()->storageType(*context);
	});

	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	auto structType = llvm::StructType::get(*context, types);

	auto addressSpace = dataLayout.getAllocaAddrSpace();
	auto align = dataLayout.getABITypeAlign(structType);

	auto alloc = new llvm::AllocaInst(structType, addressSpace, nullptr, align, "instance", block);

	std::vector<UserTypeField *> fields;

	std::transform(begin(fieldEmbryos), end(fieldEmbryos), std::back_insert_iterator(fields), [&](auto embryo)
	{
		llvm::Value *indexes[] =
		{
			llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0),
			llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), embryo.index)
		};

		auto gep = llvm::GetElementPtrInst::CreateInBounds(alloc, indexes, embryo.field.name(), block);

		auto valueType = embryo.value.type();
		auto valueStorageType = valueType->storageType(*context);

		auto valueAlign = dataLayout.getABITypeAlign(valueStorageType);

		auto store = new llvm::StoreInst(embryo.value, gep, false, valueAlign, block);

		UNUSED(store);

		return new UserTypeField(embryo.field.name(), valueType);
	});

	auto load = new llvm::LoadInst(structType, alloc, "by_value", false, align, block);

	auto type = new UserType(prototype, structType, fields);

	values.push({ type, load });

	return m_consumer->build(entryPoint, values);
}
