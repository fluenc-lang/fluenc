#include <llvm/IR/IRBuilder.h>

#include <sstream>

#include "DzInstantiation.h"
#include "DzTypeName.h"
#include "IndexIterator.h"
#include "IPrototypeProvider.h"

#include "types/Prototype.h"
#include "types/UserType.h"

DzInstantiation::DzInstantiation(DzValue *consumer
	, IPrototypeProvider *prototypeProvider
	, const std::vector<std::string> &fields
	)
	: m_consumer(consumer)
	, m_prototypeProvider(prototypeProvider)
	, m_fields(fields)
{
}

std::vector<DzResult> DzInstantiation::build(const EntryPoint &entryPoint, Stack values) const
{
	struct FieldEmbryo
	{
		int index;

		PrototypeField field;
		TypedValue value;
	};

	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	std::unordered_map<std::string, TypedValue> valueByName;

	std::transform(begin(m_fields), end(m_fields), std::insert_iterator(valueByName, begin(valueByName)), [&](auto field)
	{
		return std::make_pair(field, values.pop());
	});

	auto prototype = m_prototypeProvider->provide(entryPoint, values);

	auto prototypeFields = prototype->fields();

	std::vector<FieldEmbryo> fieldEmbryos;

	std::transform(begin(prototypeFields), end(prototypeFields), index_iterator(), std::back_insert_iterator(fieldEmbryos), [&](auto field, auto index) -> FieldEmbryo
	{
		auto value = valueByName.find(field.name());

		if (value != valueByName.end())
		{
			return { index, field, value->second };
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

	auto intType = llvm::Type::getInt32Ty(*context);
	auto structType = llvm::StructType::get(*context, types);

	auto alloc = entryPoint.alloc(structType);

	std::vector<UserTypeField> fields;

	std::transform(begin(fieldEmbryos), end(fieldEmbryos), std::back_insert_iterator(fields), [&](auto embryo) -> UserTypeField
	{
		llvm::Value *indexes[] =
		{
			llvm::ConstantInt::get(intType, 0),
			llvm::ConstantInt::get(intType, embryo.index)
		};

		auto gep = llvm::GetElementPtrInst::CreateInBounds(alloc, indexes, embryo.field.name(), block);

		auto valueType = embryo.value.type();
		auto valueStorageType = valueType->storageType(*context);

		auto valueAlign = dataLayout.getABITypeAlign(valueStorageType);

		auto store = new llvm::StoreInst(embryo.value, gep, false, valueAlign, block);

		UNUSED(store);

		return { embryo.index, embryo.field.name(), valueType };
	});

	auto type = new UserType(prototype, structType, fields);

	values.push({ type, alloc });

	return m_consumer->build(entryPoint, values);
}
