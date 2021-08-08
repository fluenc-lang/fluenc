#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>

#include "DzExportedFunction.h"
#include "EntryPoint.h"

DzExportedFunction::DzExportedFunction(const std::string &name, DzValue *block)
	: m_name(name)
	, m_block(block)
{
}

std::string DzExportedFunction::name() const
{
	return m_name;
}

FunctionAttribute DzExportedFunction::attribute() const
{
	return FunctionAttribute::Export;
}

llvm::Value *DzExportedFunction::build(const EntryPoint &entryPoint, std::deque<llvm::Value *> &values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto returnType = llvm::Type::getInt32Ty(*context);

	std::vector<llvm::Type *> argumentTypes;

	auto functionType = llvm::FunctionType::get(returnType, argumentTypes, false);
	auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, m_name, module.get());
	auto block = llvm::BasicBlock::Create(*context, "entry");

	auto ep = entryPoint
		.withFunction(function)
		.withBlock(block);

	return m_block->build(ep, values);
}
