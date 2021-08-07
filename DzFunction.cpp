#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>

#include <iostream>
#include <iomanip>

#include "DzFunction.h"
#include "DzMember.h"
#include "DzTypeName.h"
#include "DzClosureAccessor.h"
#include "EntryPoint.h"

DzFunction::DzFunction(const std::string &name
	, const std::vector<DzMember *> &arguments
	, FunctionAttribute attribute
	, DzTypeName *returnType
	, DzValue *block
	)
	: m_name(name)
	, m_arguments(arguments)
	, m_attribute(attribute)
	, m_returnType(returnType)
	, m_block(block)
{
}

std::string DzFunction::name() const
{
	return m_name;
}

std::vector<DzMember *> DzFunction::arguments() const
{
	return m_arguments;
}

DzTypeName *DzFunction::returnType() const
{
	return m_returnType;
}

FunctionAttribute DzFunction::attribute() const
{
	return m_attribute;
}

llvm::Value *DzFunction::build(const EntryPoint &entryPoint) const
{
	auto returnType = entryPoint.returnType();
	auto module = entryPoint.module();
	auto context = entryPoint.context();

	std::vector<llvm::Type *> argumentTypes;
	std::map<std::string, DzValue *> locals;

	for (auto i = 0u; i < m_arguments.size(); i++)
	{
		auto argument = m_arguments[i];

		auto name = argument->name();
		auto type = argument->type()->type(entryPoint);

//		locals[name] = new DzLocalAccess(i);

		argumentTypes.push_back(type);
	}

	auto functionType = llvm::FunctionType::get(returnType, argumentTypes, false);
	auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, m_name, module);
	auto block = llvm::BasicBlock::Create(*context, "entry", function);

	auto nested = entryPoint
		.withLocals(locals)
		.withBlock(block)
		.withParent(&entryPoint);

	m_block->build(nested);

	verifyFunction(*function);

	return function;
}
