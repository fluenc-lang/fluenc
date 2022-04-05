#include <sstream>

#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

#include "ExportedFunction.h"
#include "DzTypeName.h"
#include "EntryPoint.h"
#include "Type.h"

ExportedFunction::ExportedFunction(const std::string &name
	, Node *block
	, ITypeName *returnType
	)
	: m_name(name)
	, m_block(block)
	, m_returnType(returnType)
{
}

std::string ExportedFunction::name() const
{
	return m_name;
}

std::vector<DzBaseArgument *> ExportedFunction::arguments() const
{
	return std::vector<DzBaseArgument *>();
}

FunctionAttribute ExportedFunction::attribute() const
{
	return FunctionAttribute::Export;
}

bool ExportedFunction::hasMatchingSignature(const EntryPoint &entryPoint, const Stack &values) const
{
	UNUSED(entryPoint);
	UNUSED(values);

	return true;
}

int8_t ExportedFunction::signatureCompatibility(const EntryPoint &entryPoint, const Stack &values) const
{
	UNUSED(entryPoint);
	UNUSED(values);

	return 0;
}

std::vector<DzResult> ExportedFunction::build(const EntryPoint &entryPoint, Stack values) const
{
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	auto returnType = m_returnType->resolve(entryPoint);
	auto storageType = returnType->storageType(*context);

	std::vector<llvm::Type *> argumentTypes;

	auto functionType = llvm::FunctionType::get(storageType, argumentTypes, false);
	auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, m_name, module.get());

	auto alloc = llvm::BasicBlock::Create(*context, "alloc", function);
	auto block = llvm::BasicBlock::Create(*context);

	linkBlocks(alloc, block);

	auto ep = entryPoint
		.withFunction(function)
		.withBlock(block)
		.withAlloc(alloc);

	auto result = m_block->build(ep, values);

	verifyFunction(*function, &llvm::errs());

	return result;
}
