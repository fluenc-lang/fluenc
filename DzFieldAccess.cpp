#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>

#include <values/UserTypeValue.h>

#include "DzFieldAccess.h"
#include "IndexIterator.h"
#include "Type.h"

#include "types/IPrototype.h"

#include "values/TypedValue.h"

DzFieldAccess::DzFieldAccess(llvm::Value *instance, const NamedValue *field, size_t index)
	: m_instance(instance)
	, m_field(field)
	, m_index(index)
{
}

std::vector<DzResult> DzFieldAccess::build(const EntryPoint &entryPoint, Stack values) const
{
	UNUSED(values);

	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto block = entryPoint.block();

	auto intType = llvm::Type::getInt32Ty(*context);

	auto fieldType = m_field->type();

	if (fieldType)
	{
		auto storageType = fieldType->storageType(*context);

		llvm::Value *indexes[] =
		{
			llvm::ConstantInt::get(intType, 0),
			llvm::ConstantInt::get(intType, m_index)
		};

		auto gep = llvm::GetElementPtrInst::CreateInBounds(m_instance, indexes, m_field->name(), block);

		auto dataLayout = module->getDataLayout();
		auto align = dataLayout.getABITypeAlign(storageType);

		auto load = new llvm::LoadInst(storageType, gep, m_field->name(), false, align, block);

		if (auto prototype = dynamic_cast<const IPrototype *>(fieldType))
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

			auto cast = new llvm::BitCastInst(load, structPtr, "cast", block);

			std::vector<const NamedValue *> fieldValues;

			std::transform(begin(fields), end(fields), index_iterator(), std::back_insert_iterator(fieldValues), [&](auto field, auto index)
			{
				auto fieldAccess = new DzFieldAccess(cast, field, index);

				return new NamedValue { field->name(), entryPoint, fieldAccess, nullptr };
			});

			values.push(new UserTypeValue { prototype, fieldValues });
		}
		else
		{
			values.push(new TypedValue { fieldType, load });
		}
	}

	return {{ entryPoint, values }};
}
