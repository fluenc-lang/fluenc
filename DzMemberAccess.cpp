#include "DzMemberAccess.h"
#include "EntryPoint.h"
#include "Type.h"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>

DzMemberAccess::DzMemberAccess(DzValue *consumer, const std::string &name)
	: m_consumer(consumer)
	, m_name(name)
{
}

std::vector<DzResult> DzMemberAccess::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto block = entryPoint.block();
	auto locals = entryPoint.locals();

	auto iterator = locals.find(m_name);

	if (iterator == locals.end())
	{
		throw new std::exception();
	}

	auto valueType = iterator->second.type();
	auto storageType = valueType->storageType(*context);

	auto dataLayout = module->getDataLayout();
	auto align = dataLayout.getABITypeAlign(storageType);

	auto load = new llvm::LoadInst(storageType, iterator->second, m_name, false, align, block);

	values.push({ valueType, load });

	return m_consumer->build(entryPoint, values);
}
