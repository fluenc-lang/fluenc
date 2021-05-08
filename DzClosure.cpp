#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>

#include <sstream>

#include "DzClosure.h"
#include "DzParameter.h"
#include "EntryPoint.h"

DzClosure::DzClosure(int id, DzValue *parent)
	: m_id(id)
	, m_parent(parent)
{
}

DzParameter *DzClosure::parameter()
{
	return new DzParameter(m_id);
}

llvm::Value *DzClosure::build(const EntryPoint &entryPoint) const
{
	std::ostringstream stream;
	stream << "closure_";
	stream << m_id;

	auto returnType = entryPoint.returnType();
	auto module = entryPoint.module();
	auto context = entryPoint.context();

	std::vector<llvm::Type *> argumentTypes = { llvm::Type::getInt32Ty(*context) };

//	std::ostringstream stream;
//	stream << m_parameter->toString(entryPoint);
//	stream << " => ";
//	stream << m_parent->toString(entryPoint);

	auto functionType = llvm::FunctionType::get(returnType, argumentTypes, false);
	auto function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, stream.str(), module);
	auto block = llvm::BasicBlock::Create(*context, "entry", function);

	auto nested = entryPoint
		.withBlock(block);

	m_parent->build(nested);

	verifyFunction(*function);

	return function;
}
