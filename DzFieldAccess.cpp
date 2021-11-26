#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>

#include "DzFieldAccess.h"
#include "Type.h"
#include "InteropHelper.h"

#include "values/NamedValue.h"

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

		auto value = InteropHelper::createReadProxy(load, fieldType, entryPoint);

		values.push(value);
	}

	return {{ entryPoint, values }};
}
