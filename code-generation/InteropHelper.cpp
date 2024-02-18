#include <numeric>

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Constants.h>

#include "InteropHelper.h"
#include "EntryPoint.h"
#include "IRBuilderEx.h"

#include "iterators/void_iterator.hpp"
#include "iterators/index_iterator.hpp"

#include "types/ProxyType.h"
#include "types/OpaquePointerType.h"
#include "types/structure_type.hpp"

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

	if (auto structure = type_cast<const structure_type *>(type))
	{
		auto fields = structure->fields(entryPoint);

		if (empty(fields))
		{
			return { entryPoint, new scalar_value { opaque_pointer_type::get(structure), value } };
		}

		std::vector<llvm::Type *> types;

		std::transform(begin(fields), end(fields), std::back_inserter(types), [&](auto field)
		{
			return field.type->storageType(*context);
		});

		auto intType = llvm::Type::getInt32Ty(*context);
		auto structType = llvm::StructType::get(*context, types);
		auto structPtr = structType->getPointerTo();

		auto cast = builder.createBitCast(value, structPtr, "cast");

                std::vector<const named_value *> field_values;

		auto [accumulatedEntryPoint, _] = std::accumulate(begin(fields), end(fields), std::make_pair(entryPoint, 0), [&](auto pair, auto field)
		{
			auto [lastEntryPoint, index] = pair;

			if (field.type)
			{
				llvm::Value *indexes[] =
				{
					llvm::ConstantInt::get(intType, 0),
					llvm::ConstantInt::get(intType, index)
				};

                                auto gep = new reference_value(field.type
					, llvm::GetElementPtrInst::CreateInBounds(structType, cast, indexes, field.name, block)
					);

				auto load = builder.createLoad(gep, field.name);

				auto [resultEntryPoint, value] = createReadProxy(*load, field.type, lastEntryPoint, ast);

                                auto namedValue = new named_value { field.name, value };

				field_values.push_back(namedValue);

				return std::make_pair(resultEntryPoint, index + 1);
			}

			throw MissingTypeDeclarationException(ast, type->name(), field.name);
		});

                return { accumulatedEntryPoint, new user_type_value { structure, field_values } };
	}
        else if (type->id() == type_id::Buffer)
	{
		auto alloc = entryPoint.alloc(type);

		IRBuilderEx builder(entryPoint);

                auto address = new scalar_value(type, value);

		builder.createStore(address, alloc);

		return { entryPoint, new BufferValue(alloc) };
	}

        return { entryPoint, new scalar_value { type, value } };
}

llvm::Value *InteropHelper::createWriteProxy(const user_type_value *userTypeValue, const EntryPoint &entryPoint)
{
	auto context = entryPoint.context();
	auto module = entryPoint.module();

	auto block = entryPoint.block();

	IRBuilderEx builder(entryPoint);

	auto dataLayout = module->getDataLayout();

	auto fields = userTypeValue->fields();

        std::vector<const scalar_value *> elementValues;

        std::transform(begin(fields), end(fields), std::back_inserter(elementValues), [&](const named_value *field) -> const scalar_value *
	{
		auto fieldValue = field->value();

                if (auto reference = value_cast<const reference_value *>(fieldValue))
		{
			return builder.createLoad(reference, field->name());
		}

                if (auto userTypeValue = value_cast<const user_type_value *>(fieldValue))
		{
                    return new scalar_value { userTypeValue->type(), createWriteProxy(userTypeValue, entryPoint) };
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

        auto proxyType = new proxy_type(structType);

	auto alloc = entryPoint.alloc(proxyType);

	std::transform(begin(elementValues), end(elementValues), fluenc::index_iterator(), void_iterator<llvm::Value *>(), [&](auto field, auto index)
	{
		llvm::Value *indexes[] =
		{
			llvm::ConstantInt::get(intType, 0),
			llvm::ConstantInt::get(intType, index)
		};

                auto gep = new reference_value(field->type()
			, llvm::GetElementPtrInst::CreateInBounds(structType, *alloc, indexes, "gep", block)
			);

		return builder.createStore(field, gep);
	});

	return builder.createBitCast(*alloc, llvm::Type::getInt8PtrTy(*context), "cast");
}
