#include <llvm/IR/IRBuilder.h>

#include <types/StringType.h>

#include "DzImportedFunction.h"
#include "DzTypeName.h"
#include "DzArgument.h"
#include "AllIterator.h"
#include "Type.h"
#include "IndexIterator.h"
#include "DzFieldAccess.h"

#include "types/VoidType.h"
#include "types/UserType.h"
#include "types/Prototype.h"

#include "values/TypedValue.h"
#include "values/UserTypeValue.h"

DzImportedFunction::DzImportedFunction(const std::string &name
	, const std::vector<DzBaseArgument *> &arguments
	, DzTypeName *returnType
	)
	: m_name(name)
	, m_arguments(arguments)
	, m_returnType(returnType)
{
}

std::string DzImportedFunction::name() const
{
	return m_name;
}

FunctionAttribute DzImportedFunction::attribute() const
{
	return FunctionAttribute::Import;
}

bool DzImportedFunction::hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const
{
	if (m_arguments.size() != values.size())
	{
		return false;
	}

	auto result = true;

	std::transform(begin(m_arguments), end(m_arguments), values.rbegin(), all_true(result), [=](auto argument, auto value)
	{
		if (!value)
		{
			return false;
		}

		auto argumentType = argument->type(entryPoint);
		auto valueType = value->type();

		return valueType->is(argumentType, entryPoint);
	});

	return result;
}

llvm::Value *k(const UserTypeValue *userTypeValue, const EntryPoint &entryPoint)
{
	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	struct Kask
	{
		std::string name;
		const TypedValue *value;
	};

	auto fields = userTypeValue->fields();

	std::vector<Kask> elementValues;

	std::transform(begin(fields), end(fields), std::back_insert_iterator(elementValues), [&](const NamedValue *field) -> Kask
	{
		auto fieldResults = field->build(block, Stack());
		auto &[_, fieldValues] = *fieldResults.begin();

		auto fieldValue = fieldValues.pop();

		if (auto typedValue = dynamic_cast<const TypedValue *>(fieldValue))
		{
			return { field->name(), typedValue };
		}
		else if (auto userTypeValue = dynamic_cast<const UserTypeValue *>(fieldValue))
		{
			auto l = k(userTypeValue, entryPoint);

			return { field->name(), new TypedValue { userTypeValue->type(), l } };
		}

		throw new std::exception();
	});

	std::vector<llvm::Type *> elementTypes;

	std::transform(begin(elementValues), end(elementValues), std::back_insert_iterator(elementTypes), [&](auto value)
	{
		auto type = value.value->type();

		return type->storageType(*context);
	});

	auto structType = llvm::StructType::get(*context, elementTypes);

	auto alloc = entryPoint.alloc(structType);

	auto intType = llvm::Type::getInt32Ty(*context);

	auto i = 0;

	for (auto &field : elementValues)
	{
		llvm::Value *indexes[] =
		{
			llvm::ConstantInt::get(intType, 0),
			llvm::ConstantInt::get(intType, i++)
		};

		auto gep = llvm::GetElementPtrInst::CreateInBounds(alloc, indexes, field.name, block);

		auto type = field.value->type();
		auto storageType = type->storageType(*context);

		auto align = dataLayout.getABITypeAlign(storageType);

		auto store = new llvm::StoreInst(*field.value, gep,false, align, block);

		UNUSED(store);
	}

	auto cast = new llvm::BitCastInst(alloc, llvm::Type::getInt8PtrTy(*context), "cast", block);

	return cast;
}

std::vector<DzResult> DzImportedFunction::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto block = entryPoint.block();

	auto dataLayout = module->getDataLayout();

	auto returnType = m_returnType->resolve(entryPoint);

	std::vector<llvm::Type *> argumentTypes;
	std::vector<llvm::Value *> argumentValues;

	for (const auto &argument : m_arguments)
	{
		if (auto standardArgument  = dynamic_cast<DzArgument *>(argument))
		{
			auto name = standardArgument->name();
			auto type = standardArgument->type(entryPoint);

			auto storageType = type->storageType(*context);

			argumentTypes.push_back(storageType);

			auto value = values.pop();

			if (auto addressOfArgument = dynamic_cast<const TypedValue *>(value))
			{
				auto align = dataLayout.getABITypeAlign(storageType);

				auto load = new llvm::LoadInst(storageType, *addressOfArgument, name, false, align, block);

				argumentValues.push_back(load);
			}
			else if (auto userTypeValue = dynamic_cast<const UserTypeValue *>(value))
			{
				auto cast = k(userTypeValue, entryPoint);

				argumentValues.push_back(cast);
			}
		}
		else
		{
			throw new std::exception();
		}
	}

	llvm::FunctionType *functionType = llvm::FunctionType::get(returnType->storageType(*context), argumentTypes, false);

	auto function = module->getOrInsertFunction(m_name, functionType);

	llvm::IRBuilder<> builder(block);

	auto call = builder.CreateCall(function, argumentValues);

	if (returnType != VoidType::instance())
	{
		if (auto prototype = dynamic_cast<const IPrototype *>(returnType))
		{
			auto fields = prototype->fields(entryPoint);

			std::vector<llvm::Type *> types;

			std::transform(begin(fields), end(fields), std::back_insert_iterator(types), [&](auto field)
			{
				auto type = field->type();

				return type->storageType(*context);
			});

			auto structType = llvm::StructType::get(*context, types);
			auto structPtr = structType->getPointerTo();

			auto cast = new llvm::BitCastInst(call, structPtr, "cast", block);

			std::vector<const NamedValue *> namedValues;

			std::transform(begin(fields), end(fields), index_iterator(), std::back_insert_iterator(namedValues), [&](auto field, auto index)
			{
				auto fieldAccess = new DzFieldAccess(cast, field, index);

				return new NamedValue { field->name(), entryPoint, fieldAccess, nullptr };
			});

			auto utv = new UserTypeValue(prototype, namedValues);

			values.push(utv);
		}
		else
		{
			values.push(new TypedValue { returnType, call });
		}
	}

	return {{ entryPoint, values }};
}
