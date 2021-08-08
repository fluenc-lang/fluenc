#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>

#include "DzEntryPointNg.h"
#include "EntryPointInfo.h"
#include "DzMemberNg.h"
#include "DzTypeNameNg.h"

DzEntryPointNg::DzEntryPointNg(DzValueNg *block
	, DzTypeNameNg *returnType
	, const std::string &name
	, const std::vector<DzMemberNg *> &arguments
	)
	: m_block(block)
	, m_returnType(returnType)
	, m_name(name)
	, m_arguments(arguments)
{
}

std::string DzEntryPointNg::name() const
{
	return m_name;
}

std::vector<DzMemberNg *> DzEntryPointNg::arguments() const
{
	return m_arguments;
}

FunctionAttribute DzEntryPointNg::attribute() const
{
	return FunctionAttribute::Export;
}

DzTypeNameNg *DzEntryPointNg::returnType() const
{
	return m_returnType;
}

llvm::Value *DzEntryPointNg::build(const EntryPointInfo &entryPoint) const
{
	auto returnType = entryPoint.returnType()->build(entryPoint);
	auto &module = entryPoint.module();
	auto &context = entryPoint.context();

	std::vector<llvm::Type *> argumentTypes;

	for (auto i = 0u; i < m_arguments.size(); i++)
	{
		auto argument = m_arguments[i];

		//				auto name = argument->name();
		auto type = argument->type()->build(entryPoint);

		//		locals[name] = new DzLocalAccess(i);

		argumentTypes.push_back(type);
	}

	auto functionType = llvm::FunctionType::get(returnType, argumentTypes, false);
	auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, m_name, module.get());
	auto block = llvm::BasicBlock::Create(*context, "entry", function);

	auto ep = entryPoint
			.withBlock(block)
			.withFunction(function)
			.withName(m_name);

	auto value = m_block->build(ep);

	llvm::IRBuilder<> builder(block);

	auto load = builder.CreateLoad(llvm::Type::getInt32Ty(*context), value);

	builder.CreateRet(load);

	for (auto i = function->begin(), j = std::next(i)
		 ; i != function->end() && j != function->end()
		 ; i++, j++
		 )
	{
		llvm::BranchInst::Create(&*j, &*i);
	}

	verifyFunction(*function);

	return function;
}
