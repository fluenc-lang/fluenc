#include "EntryPoint.h"
#include "DzFunction.h"
#include "DzTypeName.h"

EntryPoint::EntryPoint(const EntryPoint *parent
	, llvm::BasicBlock *block
	, llvm::LLVMContext *context
	, llvm::Module *module
	, llvm::Value *closure
	, DzTypeName *returnType
	, const std::map<std::string, DzFunction *> &functions
	, const std::map<std::string, DzValue *> &locals
	)
	: m_parent(parent)
	, m_block(block)
	, m_context(context)
	, m_module(module)
	, m_closure(closure)
	, m_returnType(returnType)
	, m_functions(functions)
	, m_locals(locals)
{
}

const EntryPoint *EntryPoint::parent() const
{
	return m_parent;
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

llvm::Value *EntryPoint::closure() const
{
	return m_closure;
}

std::map<std::string, DzFunction *> EntryPoint::functions() const
{
	return m_functions;
}

std::map<std::string, DzValue *> EntryPoint::locals() const
{
	return m_locals;
}

EntryPoint EntryPoint::withLocals(const std::map<std::string, DzValue *> &locals) const
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

EntryPoint EntryPoint::withClosure(llvm::Value *closure) const
{
	EntryPoint clone(*this);
	clone.m_closure = closure;

	return clone;
}

EntryPoint EntryPoint::withParent(const EntryPoint *parent) const
{
	EntryPoint clone(*this);
	clone.m_parent = parent;

	return clone;
}
