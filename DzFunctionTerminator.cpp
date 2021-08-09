#include <sstream>

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>

#include "DzFunctionTerminator.h"
#include "EntryPoint.h"

DzFunctionTerminator::DzFunctionTerminator(const std::string &name)
	: m_name(name)
{
}

Stack DzFunctionTerminator::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &context = entryPoint.context();
	auto &module = entryPoint.module();

	auto function = entryPoint.function();
	auto block = entryPoint.block();

	block->setName(m_name);
	block->insertInto(function);

	std::ostringstream stream;
	stream << m_name;
	stream << "_ret";

	auto globalName = stream.str();

	auto global = module->getOrInsertGlobal(globalName, llvm::Type::getInt32Ty(*context), [&]
	{
		auto initializer = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);

		return new llvm::GlobalVariable(*module, llvm::Type::getInt32Ty(*context), false, llvm::GlobalValue::InternalLinkage, initializer, globalName);
	});

	auto value = values.pop();

	llvm::IRBuilder<> builder(block);

	builder.CreateStore(value, global);

	auto load = builder.CreateLoad(llvm::Type::getInt32Ty(*context), global);

	values.push(load);

	return values;
}
