#include "EntryPointInfo.h"

EntryPointInfo::EntryPointInfo(DzTypeNameNg *returnType, std::unique_ptr<llvm::LLVMContext> &context, std::unique_ptr<llvm::Module> &module, llvm::BasicBlock *block, llvm::Function *function, const std::map<std::string, DzCallable *> &functions, const std::map<std::string, llvm::Value *> &locals, const std::string &name)
	: m_returnType(returnType)
	, m_context(context)
	, m_module(module)
	, m_block(block)
	, m_function(function)
	, m_functions(functions)
	, m_locals(locals)
	, m_name(name)
{
}

DzTypeNameNg *EntryPointInfo::returnType() const
{
	return m_returnType;
}

std::unique_ptr<llvm::LLVMContext> &EntryPointInfo::context() const
{
	return m_context;
}

std::unique_ptr<llvm::Module> &EntryPointInfo::module() const
{
	return m_module;
}

llvm::BasicBlock *EntryPointInfo::block() const
{
	return m_block;
}

llvm::Function *EntryPointInfo::function() const
{
	return m_function;
}

std::map<std::string, DzCallable *> EntryPointInfo::functions() const
{
	return m_functions;
}

std::map<std::string, llvm::Value *> EntryPointInfo::locals() const
{
	return m_locals;
}

std::string EntryPointInfo::name() const
{
	return m_name;
}

EntryPointInfo EntryPointInfo::withBlock(llvm::BasicBlock *block) const
{
	return EntryPointInfo(m_returnType
		, m_context
		, m_module
		, block
		, m_function
		, m_functions
		, m_locals
		, m_name
		);
}

EntryPointInfo EntryPointInfo::withFunction(llvm::Function *function) const
{
	return EntryPointInfo(m_returnType
		, m_context
		, m_module
		, m_block
		, function
		, m_functions
		, m_locals
		, m_name
		);
}

EntryPointInfo EntryPointInfo::withLocals(const std::map<std::string, llvm::Value *> &locals) const
{
	return EntryPointInfo(m_returnType
		, m_context
		, m_module
		, m_block
		, m_function
		, m_functions
		, locals
		, m_name
		);
}

EntryPointInfo EntryPointInfo::withName(const std::string &name) const
{
	return EntryPointInfo(m_returnType
		, m_context
		, m_module
		, m_block
		, m_function
		, m_functions
		, m_locals
		, name
		);
}
