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

class DzLocalAccess : public DzValue
{
	public:
		DzLocalAccess(size_t index)
			: m_index(index)
		{
		}

		llvm::Value *build(const EntryPoint &entryPoint) const override
		{
			auto closure = entryPoint.closure();
			auto block = entryPoint.block();

			auto pet = closure->getType()->getPointerElementType();

			auto name = pet->getStructName();

			llvm::IRBuilder<> builder(block);

			auto k = builder.CreateStructGEP(closure, m_index);

			auto memberType = k->getType();

			return builder.CreateLoad(memberType->getPointerElementType(), k);
		}

	private:
		size_t m_index;
};

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

		locals[name] = new DzLocalAccess(i);

		argumentTypes.push_back(type);
	}

//	if (parent)
//	{
//		auto parentLocals = parent->locals();

//		for (auto &local : parentLocals)
//		{
//			locals[local.first] = new DzClosureAccessor(local.second);
//		}
//	}

	auto functionType = llvm::FunctionType::get(returnType, argumentTypes, false);
	auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, m_name, module);

	static int i;

	std::ostringstream stream;
	stream << "data_";
	stream << i++;

	auto closureType = llvm::StructType::create(*context, argumentTypes, stream.str());
	auto block = llvm::BasicBlock::Create(*context, "entry", function);

	llvm::IRBuilder<> builder(block);

	auto alloc = builder.CreateAlloca(closureType);

	for (size_t i = 0; i < function->arg_size(); i++)
	{
		auto arg = function->getArg(i);

		builder.CreateStore(arg
			, builder.CreateStructGEP(alloc, i)
			);
	}

	auto nested = entryPoint
		.withClosure(alloc)
		.withLocals(locals)
		.withBlock(block)
		.withParent(&entryPoint);

	m_block->build(nested);

	verifyFunction(*function);

	return function;
}
