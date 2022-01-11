#include <numeric>

#include <llvm/IR/Instructions.h>

#include "InteropHelper.h"
#include "IndexIterator.h"
#include "EntryPoint.h"
#include "DzFieldAccess.h"
#include "VoidIterator.h"
#include "IRBuilderEx.h"

#include "types/IPrototype.h"

#include "values/NamedValue.h"
#include "values/TypedValue.h"
#include "values/UserTypeValue.h"
#include "values/ReferenceValue.h"

const BaseValue *InteropHelper::createReadProxy(llvm::Value *value, const Type *type, const EntryPoint &entryPoint)
{
	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto block = entryPoint.block();

	IRBuilderEx builder(entryPoint);

	if (auto prototype = dynamic_cast<const IPrototype *>(type))
	{
		auto fields = prototype->fields(entryPoint);

		std::vector<llvm::Type *> types;

		std::transform(begin(fields), end(fields), std::back_inserter(types), [&](auto field)
		{
			auto type = field.type();

			return type->storageType(*context);
		});

		auto intType = llvm::Type::getInt32Ty(*context);
		auto structType = llvm::StructType::get(*context, types);
		auto structPtr = structType->getPointerTo();

		auto cast = builder.createBitCast(value, structPtr, "cast");

		std::vector<const NamedValue *> fieldValues;

		std::transform(begin(fields), end(fields), index_iterator(), std::back_inserter(fieldValues), [&](auto field, auto index)
		{
			auto fieldType = field.type();

			if (fieldType)
			{
				auto storageType = fieldType->storageType(*context);

				llvm::Value *indexes[] =
				{
					llvm::ConstantInt::get(intType, 0),
					llvm::ConstantInt::get(intType, index)
				};

				auto gep = llvm::GetElementPtrInst::CreateInBounds(cast, indexes, field.name(), block);

				auto dataLayout = module->getDataLayout();
				auto align = dataLayout.getABITypeAlign(storageType);

				auto load = builder.createLoad(gep, field.name());

				auto value = createReadProxy(load, fieldType, entryPoint);

				return new NamedValue { field.name(), value };
			}

			throw new std::exception();
		});

		return new UserTypeValue { prototype, fieldValues };
	}

	return new TypedValue { type, value };
}

llvm::Value *InteropHelper::createWriteProxy(const UserTypeValue *userTypeValue, const EntryPoint &entryPoint)
{
	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto block = entryPoint.block();

	IRBuilderEx builder(entryPoint);

	auto dataLayout = module->getDataLayout();

	auto fields = userTypeValue->fields();

	std::vector<const TypedValue *> elementValues;

	std::transform(begin(fields), end(fields), std::back_inserter(elementValues), [&](const NamedValue *field) -> const TypedValue *
	{
		auto fieldValue = field->value();

		if (auto reference = dynamic_cast<const ReferenceValue *>(fieldValue))
		{
			auto type = reference->type();

			auto load = builder.createLoad(*reference, field->name());

			return new TypedValue { type, load };
		}

		if (auto userTypeValue = dynamic_cast<const UserTypeValue *>(fieldValue))
		{
			return new TypedValue { userTypeValue->type(), createWriteProxy(userTypeValue, entryPoint) };
		}

		throw new std::exception();
	});

	std::vector<llvm::Type *> elementTypes;

	std::transform(begin(elementValues), end(elementValues), std::back_inserter(elementTypes), [&](auto value)
	{
		auto type = value->type();

		return type->storageType(*context);
	});

	auto intType = llvm::Type::getInt32Ty(*context);
	auto structType = llvm::StructType::get(*context, elementTypes);

	auto alloc = entryPoint.alloc(structType);

	std::transform(begin(elementValues), end(elementValues), index_iterator(), void_iterator<llvm::Value *>(), [&](auto field, auto index)
	{
		llvm::Value *indexes[] =
		{
			llvm::ConstantInt::get(intType, 0),
			llvm::ConstantInt::get(intType, index)
		};

		auto gep = llvm::GetElementPtrInst::CreateInBounds(alloc, indexes, "gep", block);

		return builder.createStore(*field, gep);
	});

	return builder.createBitCast(alloc, llvm::Type::getInt8PtrTy(*context), "cast");
}
