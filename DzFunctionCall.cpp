#include <sstream>
#include <iterator>
#include <numeric>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Argument.h>

#include "DzFunctionCall.h"
#include "DzReturn.h"
#include "DzClosure.h"
#include "DzParameter.h"
#include "VisitorV2.h"
#include "EntryPoint.h"
#include "DzFunction.h"

DzFunctionCall::DzFunctionCall(std::string name, DzValue *parent)
	: m_parent(parent)
	, m_name(name)
{
}

void DzFunctionCall::addArgument(DzValue *argument)
{
	m_arguments.push_back(argument);
}

llvm::Value *DzFunctionCall::build(const EntryPoint &entryPoint) const
{
	auto target = getTarget(entryPoint);

	std::vector<llvm::Value *> arguments;

	for (auto argument : m_arguments)
	{
		auto value = argument->build(entryPoint);

		arguments.push_back(value);
	}

	if (m_parent)
	{
		auto consumer = m_parent->build(entryPoint);

		arguments.push_back(consumer);
	}

	llvm::IRBuilder<> builder(entryPoint.block());

	return builder.CreateCall(target, arguments);
}

llvm::FunctionCallee DzFunctionCall::getTarget(const EntryPoint &entryPoint) const
{
	auto locals = entryPoint.locals();
	auto functions = entryPoint.functions();
	auto module = entryPoint.module();

	auto local = locals.find(m_name);

	if (local != locals.end())
	{
		auto functionPointer = local->second;

		auto type = functionPointer->getType();
		auto pet = type->getPointerElementType();
		auto functionType = llvm::cast<llvm::FunctionType>(pet);

		return llvm::FunctionCallee(functionType, functionPointer);
	}

	auto existing = module->getFunction(m_name);

	if (existing)
	{
		return existing;
	}

	auto dependency = functions[m_name];

	if (dependency)
	{
		return (llvm::Function *)dependency->build(entryPoint);
	}

	return nullptr;
}
