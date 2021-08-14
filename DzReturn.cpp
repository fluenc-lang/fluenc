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

std::vector<DzResult> DzReturn::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

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

	auto type = value->getType();

	auto global = module->getOrInsertGlobal(globalName, type, [&]
	{
		auto initializer = llvm::ConstantInt::get(type, 0);

		return new llvm::GlobalVariable(*module, type, false, llvm::GlobalValue::InternalLinkage, initializer, globalName);
	});

	llvm::IRBuilder<> builder(block);

	builder.CreateStore(value, global);

	auto load = builder.CreateLoad(type, global);

	values.push(load);

	return m_consumer->build(entryPoint, values);
}
