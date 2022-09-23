#include <llvm/IR/IRBuilder.h>

#include "EntryPoint.h"
#include "Type.h"

#include "values/ReferenceValue.h"

EntryPoint::EntryPoint(int depth
	, int index
	, const EntryPoint *parent
	, const EntryPoint *entry
	, llvm::BasicBlock *block
	, llvm::BasicBlock *alloc
	, llvm::Function *function
	, llvm::Module *module
	, llvm::LLVMContext *context
	, const std::string &name
	, immer::box<std::multimap<std::string, const CallableNode *> > functions
	, immer::box<std::map<std::string, const BaseValue *>> locals
	, immer::box<std::map<std::string, const Node *>> globals
	, immer::box<std::map<std::string, Prototype *>> types
	, immer::box<Stack> values
	, IIteratorStorage *iteratorStorage
	)
	: m_depth(depth)
	, m_index(index)
	, m_parent(parent)
	, m_entry(entry)
	, m_block(block)
	, m_alloc(alloc)
	, m_function(function)
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

void EntryPoint::incorporate()
{
	auto insert = [&](llvm::BasicBlock *block, auto recurse) -> void
	{
		if (block->getParent())
		{
			return;
		}

		block->insertInto(m_function);

		for (auto successor : llvm::successors(block))
		{
			recurse(successor, recurse);
		}
	};

	insert(m_alloc, insert);
}

int EntryPoint::depth() const
{
	return m_depth;
}

int EntryPoint::index() const
{
	return m_index;
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

const ReferenceValue *EntryPoint::alloc(const Type *type, const llvm::Twine &name) const
{
	auto storageType = type->storageType(*m_context);

	auto dataLayout = m_module->getDataLayout();

	auto align = dataLayout.getPrefTypeAlign(storageType);
	auto addressSpace = dataLayout.getAllocaAddrSpace();

	return new ReferenceValue(type
		, new llvm::AllocaInst(storageType, addressSpace, nullptr, align, name, &*m_alloc->begin())
		);
}

llvm::Module *EntryPoint::module() const
{
	return m_module;
}

llvm::LLVMContext *EntryPoint::context() const
{
	return m_context;
}

std::string EntryPoint::name() const
{
	return m_name;
}

const std::multimap<std::string, const CallableNode *> &EntryPoint::functions() const
{
	return m_functions.get();
}

const std::map<std::string, const BaseValue *> &EntryPoint::locals() const
{
	return m_locals.get();
}

const std::map<std::string, const Node *> &EntryPoint::globals() const
{
	return m_globals.get();
}

const std::map<std::string, Prototype *> &EntryPoint::types() const
{
	return m_types.get();
}

const Stack &EntryPoint::values() const
{
	return m_values.get();
}

IIteratorStorage *EntryPoint::iteratorStorage() const
{
	return m_iteratorStorage;
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
	return EntryPoint(m_depth + 1
		, m_index
		, new EntryPoint(*this)
		, m_entry
		, block
		, m_alloc
		, m_function
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
		, m_index
		, m_parent
		, m_entry
		, m_block
		, alloc
		, m_function
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

EntryPoint EntryPoint::withIteratorStorage(IIteratorStorage *iteratorStorage) const
{
	return EntryPoint(m_depth + 1
		, m_index
		, m_parent
		, m_entry
		, m_block
		, m_alloc
		, m_function
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
		, m_index
		, new EntryPoint(*this)
		, new EntryPoint(*this)
		, m_block
		, m_alloc
		, m_function
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
		, m_index
		, m_parent
		, m_entry
		, m_block
		, m_alloc
		, function
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
		, m_index
		, m_parent
		, m_entry
		, m_block
		, m_alloc
		, m_function
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
		, m_index
		, new EntryPoint(*this)
		, m_entry
		, m_block
		, m_alloc
		, m_function
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

EntryPoint EntryPoint::withValues(const Stack &values) const
{
	return EntryPoint(m_depth + 1
		, m_index
		, m_parent
		, m_entry
		, m_block
		, m_alloc
		, m_function
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
		, m_index
		, m_parent
		, m_entry
		, m_block
		, m_alloc
		, m_function
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

EntryPoint EntryPoint::withIndex(int index) const
{
	return EntryPoint(m_depth
		, index
		, m_parent
		, m_entry
		, m_block
		, m_alloc
		, m_function
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
