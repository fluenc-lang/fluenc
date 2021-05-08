#include "EntryPoint.h"
#include "DzFunction.h"
#include "DzTypeName.h"

EntryPoint::EntryPoint(llvm::BasicBlock *block
	, llvm::LLVMContext *context
	, llvm::Module *module
	, DzTypeName *returnType
	, const std::map<std::string, DzFunction *> &functions
	, const std::map<std::string, llvm::Value *> &locals
	)
	: m_block(block)
	, m_context(context)
	, m_module(module)
	, m_returnType(returnType)
	, m_functions(functions)
	, m_locals(locals)
{
}

llvm::BasicBlock *EntryPoint::block() const
{
	return m_block;
}

llvm::LLVMContext *EntryPoint::context() const
{
	return m_context;
}

llvm::Module *EntryPoint::module() const
{
	return m_module;
}

llvm::Type *EntryPoint::returnType() const
{
	return m_returnType->type(*this);
}

std::map<std::string, DzFunction *> EntryPoint::functions() const
{
	return m_functions;
}

std::map<std::string, llvm::Value *> EntryPoint::locals() const
{
	return m_locals;
}

EntryPoint EntryPoint::withLocals(const std::map<std::string, llvm::Value *> &locals) const
{
	EntryPoint clone(*this);
	clone.m_locals = locals;

	return clone;
}

EntryPoint EntryPoint::withBlock(llvm::BasicBlock *block) const
{
	EntryPoint clone(*this);
	clone.m_block = block;

	return clone;
}
