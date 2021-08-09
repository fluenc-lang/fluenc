#include "EntryPoint.h"

EntryPoint::EntryPoint(llvm::BasicBlock *block
	, llvm::Function *function
	, std::unique_ptr<llvm::Module> &module
	, std::unique_ptr<llvm::LLVMContext> &context
	, const std::multimap<std::string, DzCallable *> &functions
	, const std::map<std::string, llvm::Value *> &locals
	)
	: m_block(block)
	, m_function(function)
	, m_module(module)
	, m_context(context)
	, m_functions(functions)
	, m_locals(locals)
{
}

llvm::BasicBlock *EntryPoint::block() const
{
	return m_block;
}

llvm::Function *EntryPoint::function() const
{
	return m_function;
}

std::unique_ptr<llvm::Module> &EntryPoint::module() const
{
	return m_module;
}

std::unique_ptr<llvm::LLVMContext> &EntryPoint::context() const
{
	return m_context;
}

std::multimap<std::string, DzCallable *> EntryPoint::functions() const
{
	return m_functions;
}

std::map<std::string, llvm::Value *> EntryPoint::locals() const
{
	return m_locals;
}

EntryPoint EntryPoint::withBlock(llvm::BasicBlock *block) const
{
	return EntryPoint(block
		, m_function
		, m_module
		, m_context
		, m_functions
		, m_locals
		);
}

EntryPoint EntryPoint::withFunction(llvm::Function *function) const
{
	return EntryPoint(m_block
		, function
		, m_module
		, m_context
		, m_functions
		, m_locals
		);
}

EntryPoint EntryPoint::withLocals(const std::map<std::string, llvm::Value *> &locals) const
{
	return EntryPoint(m_block
		, m_function
		, m_module
		, m_context
		, m_functions
		, locals
		);
}
