#include <sstream>

#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>

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

	auto returnType = llvm::Type::getInt32Ty(*context);

	std::vector<llvm::Type *> argumentTypes;

	auto functionType = llvm::FunctionType::get(returnType, argumentTypes, false);
	auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, m_name, module.get());
	auto block = llvm::BasicBlock::Create(*context);

	std::ostringstream stream;
	stream << m_name;
	stream << "_ret";

	auto globalName = stream.str();

	auto global = module->getOrInsertGlobal(globalName, llvm::Type::getInt32Ty(*context), [&]
	{
		auto initializer = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 0);

		return new llvm::GlobalVariable(*module, llvm::Type::getInt32Ty(*context), false, llvm::GlobalValue::InternalLinkage, initializer, globalName);
	});

	auto ep = entryPoint
		.withFunction(function)
		.withBlock(block)
		.withReturnValueAddress(global);

	auto result = m_block->build(ep, values);

	auto retBlock = llvm::BasicBlock::Create(*context, "exit", function);

	for (auto &[iep, _] : result)
	{
		llvm::BranchInst::Create(retBlock, iep.block());
	}

	llvm::IRBuilder<> builder(retBlock);
	auto load = builder.CreateLoad(llvm::Type::getInt32Ty(*context), global);
	builder.CreateRet(load);

	if (verifyFunction(*function, &llvm::errs()))
	{
//		throw new std::exception();
	}

	return result;
}
