#include <sstream>

#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRBuilder.h>

#include "DzReturn.h"
#include "EntryPoint.h"

DzReturn::DzReturn(DzValue *consumer)
	: m_consumer(consumer)
{
}

llvm::Constant *getInitializer(llvm::Type *type)
{
	if (type->isStructTy())
	{
		return llvm::ConstantAggregateZero::get(type);
	}

	return llvm::ConstantInt::get(type, 0);
}

std::vector<DzResult> DzReturn::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto name = entryPoint.name();
	auto function = entryPoint.function();
	auto block = entryPoint.block();

	block->setName(name);
	block->insertInto(function);

	std::ostringstream stream;
	stream << block->getName();
	stream << "_ret";

	auto globalName = stream.str();
	auto value = values.pop();

	auto type = value.type();
	auto storageType = type->storageType(*context);

	auto global = module->getOrInsertGlobal(globalName, storageType, [&]
	{
		auto initializer = getInitializer(storageType);

		return new llvm::GlobalVariable(*module, storageType, false, llvm::GlobalValue::InternalLinkage, initializer, globalName);
	});

	llvm::IRBuilder<> builder(block);

	builder.CreateStore(value, global);

	auto load = builder.CreateLoad(storageType, global);

	values.push(TypedValue(type, load));

	return m_consumer->build(entryPoint, values);
}
