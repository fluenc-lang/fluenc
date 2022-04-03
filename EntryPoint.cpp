#include <llvm/IR/IRBuilder.h>

#include "EntryPoint.h"

EntryPoint::EntryPoint(int depth
	, const EntryPoint *parent
	, const EntryPoint *entry
	, llvm::BasicBlock *block
	, llvm::BasicBlock *alloc
	, llvm::Function *function
	, llvm::Value *returnValueAddress
	, std::unique_ptr<llvm::Module> *module
	, std::unique_ptr<llvm::LLVMContext> *context
	, const std::string &name
	, const std::multimap<std::string, DzCallable *> &functions
	, const std::map<std::string, const BaseValue *> &locals
	, const std::map<std::string, const DzValue *> &globals
	, const std::map<std::string, Prototype *> &types
	, const Stack &values
	, IteratorStorage *iteratorStorage
	)
	: m_depth(depth)
	, m_parent(parent)
	, m_entry(entry)
	, m_block(block)
	, m_alloc(alloc)
	, m_function(function)
	, m_returnValueAddress(returnValueAddress)
	, m_module(module)
	, m_context(context)
	, m_name(name)
	, m_functions(functions)
	, m_locals(locals)
	, m_globals(globals)
	, m_types(types)
	, m_values(values)
	, m_iteratorStorage(iteratorStorage)
{
}

int EntryPoint::depth() const
{
	return m_depth;
}

llvm::BasicBlock *EntryPoint::block() const
{
	return m_block;
}

const EntryPoint *EntryPoint::entry() const
{
	if (m_entry)
	{
		return m_entry;
	}

	return this;
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

std::map<std::string, const BaseValue *> EntryPoint::locals() const
{
	return m_locals;
}

std::map<std::string, const DzValue *> EntryPoint::globals() const
{
	return m_globals;
}

std::map<std::string, Prototype *> EntryPoint::types() const
{
	return m_types;
}

Stack EntryPoint::values() const
{
	return m_values;
}

IteratorStorage *EntryPoint::iteratorStorage() const
{
	return m_iteratorStorage;
}

const EntryPoint *EntryPoint::byName(const std::string &name) const
{
	if (m_name == name)
	{
		return m_entry;
	}

	if (m_parent)
	{
		return m_parent->byName(name);
	}

	return nullptr;
}

EntryPoint EntryPoint::withBlock(llvm::BasicBlock *block) const
{
	return EntryPoint(m_depth + 1
		, new EntryPoint(*this)
		, m_entry
		, block
		, m_alloc
		, m_function
		, m_returnValueAddress
		, m_module
		, m_context
		, m_name
		, m_functions
		, m_locals
		, m_globals
		, m_types
		, m_values
		, m_iteratorStorage
		);
}

EntryPoint EntryPoint::withAlloc(llvm::BasicBlock *alloc) const
{
	return EntryPoint(m_depth + 1
		, m_parent
		, m_entry
		, m_block
		, alloc
		, m_function
		, m_returnValueAddress
		, m_module
		, m_context
		, m_name
		, m_functions
		, m_locals
		, m_globals
		, m_types
		, m_values
		, m_iteratorStorage
		);
}

EntryPoint EntryPoint::withIteratorStorage(IteratorStorage *iteratorStorage) const
{
	return EntryPoint(m_depth + 1
		, m_parent
		, m_entry
		, m_block
		, m_alloc
		, m_function
		, m_returnValueAddress
		, m_module
		, m_context
		, m_name
		, m_functions
		, m_locals
		, m_globals
		, m_types
		, m_values
		, iteratorStorage
		);
}

EntryPoint EntryPoint::markEntry() const
{
	return EntryPoint(m_depth + 1
		, new EntryPoint(*this)
		, new EntryPoint(*this)
		, m_block
		, m_alloc
		, m_function
		, m_returnValueAddress
		, m_module
		, m_context
		, m_name
		, m_functions
		, m_locals
		, m_globals
		, m_types
		, m_values
		, m_iteratorStorage
		);
}

EntryPoint EntryPoint::withFunction(llvm::Function *function) const
{
	return EntryPoint(m_depth + 1
		, m_parent
		, m_entry
		, m_block
		, m_alloc
		, function
		, m_returnValueAddress
		, m_module
		, m_context
		, m_name
		, m_functions
		, m_locals
		, m_globals
		, m_types
		, m_values
		, m_iteratorStorage
		);
}

EntryPoint EntryPoint::withLocals(const std::map<std::string, const BaseValue *> &locals) const
{
	return EntryPoint(m_depth + 1
		, m_parent
		, m_entry
		, m_block
		, m_alloc
		, m_function
		, m_returnValueAddress
		, m_module
		, m_context
		, m_name
		, m_functions
		, locals
		, m_globals
		, m_types
		, m_values
		, m_iteratorStorage
		);
}

EntryPoint EntryPoint::withName(const std::string &name) const
{
	return EntryPoint(m_depth + 1
		, new EntryPoint(*this)
		, m_entry
		, m_block
		, m_alloc
		, m_function
		, m_returnValueAddress
		, m_module
		, m_context
		, name
		, m_functions
		, m_locals
		, m_globals
		, m_types
		, m_values
		, m_iteratorStorage
		);
}

EntryPoint EntryPoint::withReturnValueAddress(llvm::Value *address) const
{
	return EntryPoint(m_depth + 1
		, m_parent
		, m_entry
		, m_block
		, m_alloc
		, m_function
		, address
		, m_module
		, m_context
		, m_name
		, m_functions
		, m_locals
		, m_globals
		, m_types
		, m_values
		, m_iteratorStorage
		);
}

EntryPoint EntryPoint::withValues(const Stack &values) const
{
	return EntryPoint(m_depth + 1
		, m_parent
		, m_entry
		, m_block
		, m_alloc
		, m_function
		, m_returnValueAddress
		, m_module
		, m_context
		, m_name
		, m_functions
		, m_locals
		, m_globals
		, m_types
		, values
		, m_iteratorStorage
		);
}

EntryPoint EntryPoint::withDepth(int depth) const
{
	return EntryPoint(depth
		, m_parent
		, m_entry
		, m_block
		, m_alloc
		, m_function
		, m_returnValueAddress
		, m_module
		, m_context
		, m_name
		, m_functions
		, m_locals
		, m_globals
		, m_types
		, m_values
		, m_iteratorStorage
		);
}
