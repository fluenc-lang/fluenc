#include <llvm/IR/IRBuilder.h>

#include "EntryPoint.h"

EntryPoint::EntryPoint(const EntryPoint *parent
	, llvm::BasicBlock *block
	, llvm::BasicBlock *alloc
	, llvm::BasicBlock *entry
	, llvm::Function *function
	, llvm::Value *returnValueAddress
	, std::unique_ptr<llvm::Module> *module
	, std::unique_ptr<llvm::LLVMContext> *context
	, const std::string &name
	, const std::multimap<std::string, DzCallable *> &functions
	, const std::map<std::string, const TypedValue *> &locals
	, const std::map<std::string, Prototype *> &types
	, const Stack &values
	)
	: m_parent(parent)
	, m_block(block)
	, m_alloc(alloc)
	, m_entry(entry)
	, m_function(function)
	, m_returnValueAddress(returnValueAddress)
	, m_module(module)
	, m_context(context)
	, m_name(name)
	, m_functions(functions)
	, m_locals(locals)
	, m_types(types)
	, m_values(values)
{
}

llvm::BasicBlock *EntryPoint::block() const
{
	return m_block;
}

llvm::BasicBlock *EntryPoint::entry() const
{
	return m_entry;
}

llvm::Function *EntryPoint::function() const
{
	return m_function;
}

llvm::Value *EntryPoint::returnValueAddress() const
{
	return m_returnValueAddress;
}

llvm::Value *EntryPoint::alloc(llvm::Type *type) const
{
	llvm::IRBuilder<> builder(m_alloc, m_alloc->begin());

	return builder.CreateAlloca(type);
}

std::unique_ptr<llvm::Module> &EntryPoint::module() const
{
	return *m_module;
}

std::unique_ptr<llvm::LLVMContext> &EntryPoint::context() const
{
	return *m_context;
}

std::string EntryPoint::name() const
{
	return m_name;
}

std::multimap<std::string, DzCallable *> EntryPoint::functions() const
{
	return m_functions;
}

std::map<std::string, const TypedValue *> EntryPoint::locals() const
{
	return m_locals;
}

std::map<std::string, Prototype *> EntryPoint::types() const
{
	return m_types;
}

Stack EntryPoint::values() const
{
	return m_values;
}

const EntryPoint *EntryPoint::byName(const std::string &name) const
{
	if (m_name == name)
	{
		return this;
	}

	if (m_parent)
	{
		return m_parent->byName(name);
	}

	return nullptr;
}

EntryPoint EntryPoint::withBlock(llvm::BasicBlock *block) const
{
	auto parent = new EntryPoint(*this);

	return EntryPoint(parent
		, block
		, m_alloc
		, m_entry
		, m_function
		, m_returnValueAddress
		, m_module
		, m_context
		, m_name
		, m_functions
		, m_locals
		, m_types
		, m_values
		);
}

EntryPoint EntryPoint::withAlloc(llvm::BasicBlock *alloc) const
{
	return EntryPoint(m_parent
		, m_block
		, alloc
		, m_entry
		, m_function
		, m_returnValueAddress
		, m_module
		, m_context
		, m_name
		, m_functions
		, m_locals
		, m_types
		, m_values
		);
}

EntryPoint EntryPoint::withEntry(llvm::BasicBlock *entry) const
{
	auto parent = new EntryPoint(*this);

	return EntryPoint(parent
		, m_block
		, m_alloc
		, entry
		, m_function
		, m_returnValueAddress
		, m_module
		, m_context
		, m_name
		, m_functions
		, m_locals
		, m_types
		, m_values
		);
}

EntryPoint EntryPoint::withFunction(llvm::Function *function) const
{
	return EntryPoint(m_parent
		, m_block
		, m_alloc
		, m_entry
		, function
		, m_returnValueAddress
		, m_module
		, m_context
		, m_name
		, m_functions
		, m_locals
		, m_types
		, m_values
		);
}

EntryPoint EntryPoint::withLocals(const std::map<std::string, const TypedValue *> &locals) const
{
	return EntryPoint(m_parent
		, m_block
		, m_alloc
		, m_entry
		, m_function
		, m_returnValueAddress
		, m_module
		, m_context
		, m_name
		, m_functions
		, locals
		, m_types
		, m_values
		);
}

EntryPoint EntryPoint::withName(const std::string &name) const
{
	auto parent = new EntryPoint(*this);

	return EntryPoint(parent
		, m_block
		, m_alloc
		, m_entry
		, m_function
		, m_returnValueAddress
		, m_module
		, m_context
		, name
		, m_functions
		, m_locals
		, m_types
		, m_values
		);
}

EntryPoint EntryPoint::withReturnValueAddress(llvm::Value *address) const
{
	return EntryPoint(m_parent
		, m_block
		, m_alloc
		, m_entry
		, m_function
		, address
		, m_module
		, m_context
		, m_name
		, m_functions
		, m_locals
		, m_types
		, m_values
		);
}

EntryPoint EntryPoint::withValues(const Stack &values) const
{
	return EntryPoint(m_parent
		, m_block
		, m_alloc
		, m_entry
		, m_function
		, m_returnValueAddress
		, m_module
		, m_context
		, m_name
		, m_functions
		, m_locals
		, m_types
		, values
		);
}
