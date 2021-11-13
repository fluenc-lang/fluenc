#include <llvm/IR/IRBuilder.h>

#include <sstream>

#include "DzInstantiation.h"
#include "DzTypeName.h"
#include "IndexIterator.h"
#include "IPrototypeProvider.h"

#include "types/Prototype.h"
#include "types/UserType.h"

#include "values/TypedValue.h"

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
		size_t index;

		const PrototypeField field;
		const BaseValue *value;
	};

	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	std::unordered_map<std::string, const BaseValue *> valueByName;

	std::transform(begin(m_fields), end(m_fields), std::insert_iterator(valueByName, begin(valueByName)), [&](auto field)
	{
		return std::make_pair(field, values.pop());
	});

	auto prototype = m_prototypeProvider->provide(entryPoint, values);

	auto prototypeFields = prototype->fields(entryPoint);

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
			if (defaultValues.size() > 0)
			{
				return { index, field, *defaultValues.begin() };
			}

			return { index, field, nullptr };
		}

		throw new std::exception();
	});

	std::vector<llvm::Type *> types;

	std::transform(begin(fieldEmbryos), end(fieldEmbryos), std::back_insert_iterator(types), [&](auto embryo) -> llvm::Type *
	{
		if (embryo.value)
		{
			return embryo.value->type()->storageType(*context);
		}

		return llvm::Type::getInt8PtrTy(*context);
	});

	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	auto intType = llvm::Type::getInt32Ty(*context);
	auto structType = llvm::StructType::get(*context, types);

	auto alloc = entryPoint.alloc(structType);

	std::vector<UserTypeField> fields;

	std::transform(begin(fieldEmbryos), end(fieldEmbryos), std::back_insert_iterator(fields), [&](auto embryo) -> UserTypeField
	{
		if (auto typedValue = dynamic_cast<const TypedValue *>(embryo.value))
		{
			llvm::Value *indexes[] =
			{
				llvm::ConstantInt::get(intType, 0),
				llvm::ConstantInt::get(intType, embryo.index)
			};

			auto gep = llvm::GetElementPtrInst::CreateInBounds(alloc, indexes, embryo.field.name(), block);

			auto valueType = embryo.value->type();
			auto valueStorageType = valueType->storageType(*context);

			auto valueAlign = dataLayout.getABITypeAlign(valueStorageType);

			auto store = new llvm::StoreInst(*typedValue, gep, false, valueAlign, block);

			UNUSED(store);

			return { embryo.index, embryo.field.name(), valueType };
		}
		else
		{
			return { embryo.index, embryo.field.name(), nullptr };
		}
	});

	auto type = new UserType(prototype, structType, fields);

	values.push(new TypedValue { type, alloc });

	return m_consumer->build(entryPoint, values);
}
