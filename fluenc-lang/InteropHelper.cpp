#include <numeric>

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Constants.h>

#include "InteropHelper.h"
#include "IndexIterator.h"
#include "EntryPoint.h"
#include "VoidIterator.h"
#include "IRBuilderEx.h"
#include "IPrototype.h"

#include "types/ProxyType.h"
#include "types/OpaquePointerType.h"

#include "values/NamedValue.h"
#include "values/ScalarValue.h"
#include "values/UserTypeValue.h"
#include "values/ReferenceValue.h"
#include "values/BufferValue.h"

#include "exceptions/MissingTypeDeclarationException.h"

ReadProxy InteropHelper::createReadProxy(llvm::Value *value
	, const Type *type
	, const EntryPoint &entryPoint
	, const std::shared_ptr<peg::Ast> &ast
	)
{
	auto context = entryPoint.context();

	auto block = entryPoint.block();

	IRBuilderEx builder(entryPoint);

	if (auto prototype = type_cast<const IPrototype *>(type))
	{
		Emitter emitter;

		auto [fieldsEntryPoint, fields] = prototype->fields(entryPoint, emitter);

		if (empty(fields))
		{
			return { fieldsEntryPoint, new ScalarValue { OpaquePointerType::get(prototype), value } };
		}

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

		auto [accumulatedEntryPoint, _] = std::accumulate(begin(fields), end(fields), std::make_pair(fieldsEntryPoint, 0), [&](auto pair, auto field)
		{
			auto [lastEntryPoint, index] = pair;

			auto fieldType = field.type();

			if (fieldType)
			{
				llvm::Value *indexes[] =
				{
					llvm::ConstantInt::get(intType, 0),
					llvm::ConstantInt::get(intType, index)
				};

				auto gep = new ReferenceValue(field.type()
					, llvm::GetElementPtrInst::CreateInBounds(structType, cast, indexes, field.name(), block)
					);

				auto load = builder.createLoad(gep, field.name());

				auto [resultEntryPoint, value] = createReadProxy(*load, fieldType, lastEntryPoint, ast);

				auto namedValue = new NamedValue { field.name(), value };

				fieldValues.push_back(namedValue);

				return std::make_pair(resultEntryPoint, index + 1);
			}

			throw MissingTypeDeclarationException(ast, type->name(), field.name());
		});

		return { accumulatedEntryPoint, new UserTypeValue { prototype, fieldValues } };
	}
	else if (type->id() == TypeId::Buffer)
	{
		auto alloc = entryPoint.alloc(type);

		IRBuilderEx builder(entryPoint);

		auto address = new ScalarValue(type, value);

		builder.createStore(address, alloc);

		return { entryPoint, new BufferValue(alloc) };
	}

	return { entryPoint, new ScalarValue { type, value } };
}

llvm::Value *InteropHelper::createWriteProxy(const UserTypeValue *userTypeValue, const EntryPoint &entryPoint)
{
	auto context = entryPoint.context();
	auto module = entryPoint.module();

	auto block = entryPoint.block();

	IRBuilderEx builder(entryPoint);

	auto dataLayout = module->getDataLayout();

	auto fields = userTypeValue->fields();

	std::vector<const ScalarValue *> elementValues;

	std::transform(begin(fields), end(fields), std::back_inserter(elementValues), [&](const NamedValue *field) -> const ScalarValue *
	{
		auto fieldValue = field->value();

		if (auto reference = value_cast<const ReferenceValue *>(fieldValue))
		{
			return builder.createLoad(reference, field->name());
		}

		if (auto userTypeValue = value_cast<const UserTypeValue *>(fieldValue))
		{
			return new ScalarValue { userTypeValue->type(), createWriteProxy(userTypeValue, entryPoint) };
		}

		throw std::exception();
	});

	std::vector<llvm::Type *> elementTypes;

	std::transform(begin(elementValues), end(elementValues), std::back_inserter(elementTypes), [&](auto value)
	{
		auto type = value->type();

		return type->storageType(*context);
	});

	auto intType = llvm::Type::getInt32Ty(*context);
	auto structType = llvm::StructType::get(*context, elementTypes);

	auto proxyType = new ProxyType(structType);

	auto alloc = entryPoint.alloc(proxyType);

	std::transform(begin(elementValues), end(elementValues), index_iterator(), void_iterator<llvm::Value *>(), [&](auto field, auto index)
	{
		llvm::Value *indexes[] =
		{
			llvm::ConstantInt::get(intType, 0),
			llvm::ConstantInt::get(intType, index)
		};

		auto gep = new ReferenceValue(field->type()
			, llvm::GetElementPtrInst::CreateInBounds(structType, *alloc, indexes, "gep", block)
			);

		return builder.createStore(field, gep);
	});

	return builder.createBitCast(*alloc, llvm::Type::getInt8PtrTy(*context), "cast");
}
