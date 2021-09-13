#include <sstream>

#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include "DzExportedFunction.h"
#include "DzTypeName.h"
#include "EntryPoint.h"

DzExportedFunction::DzExportedFunction(const std::string &name
	, DzValue *block
	, DzTypeName *returnType
	)
	: m_name(name)
	, m_block(block)
	, m_returnType(returnType)
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

bool DzExportedFunction::hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values, size_t numberOfArguments) const
{
	UNUSED(entryPoint);
	UNUSED(values);
	UNUSED(numberOfArguments);

	return true;
}

std::vector<DzResult> DzExportedFunction::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto returnType = m_returnType->resolve(entryPoint);
	auto storageType = returnType->storageType(*context);

	std::vector<llvm::Type *> argumentTypes;

	auto functionType = llvm::FunctionType::get(storageType, argumentTypes, false);
	auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, m_name, module.get());
	auto block = llvm::BasicBlock::Create(*context);

	auto ep = entryPoint
		.withFunction(function)
		.withBlock(block);

	auto result = m_block->build(ep, values);

	verifyFunction(*function, &llvm::errs());

	return result;
}
