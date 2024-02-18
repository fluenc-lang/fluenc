#include <llvm/IR/IRBuilder.h>

#include "EntryPoint.h"
#include "Type.h"

#include "types/Int32Type.h"
#include "types/Int64Type.h"
#include "types/Uint32Type.h"
#include "types/BooleanType.h"
#include "types/StringType.h"
#include "types/VoidType.h"
#include "types/WithoutType.h"
#include "types/ByteType.h"
#include "types/AnyType.h"
#include "types/Float32Type.h"
#include "types/IteratorType.h"
#include "types/BufferType.h"
#include "types/structure_type.hpp"

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
	, immer::box<std::multimap<std::string, fluenc::function_node *> > functions
	, immer::box<std::map<std::string, const BaseValue *>> locals
                       , immer::box<std::map<std::string, fluenc::expression_t> > globals
					   , immer::box<std::map<std::string, fluenc::struct_node *> > types
					   , immer::box<std::vector<const fluenc::function_node *> > roots
	, immer::box<Stack> values
					   , const expanded_type *iteratorType
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
	, m_roots(roots)
	, m_values(values)
	, m_iteratorType(iteratorType)
{
}

Type *EntryPoint::resolve(const std::string &type_name) const
{
	if (type_name == "i32")
	{
            return int32_type::instance();
	}

	if (type_name == "i64")
	{
            return int64_type::instance();
	}

	if (type_name == "u32")
	{
            return uint32_type::instance();
	}

	if (type_name == "u8")
	{
            return byte_type::instance();
	}

	if (type_name == "f32")
	{
            return float32_type::instance();
	}

	if (type_name == "bool")
	{
            return boolean_type::instance();
	}

	if (type_name == "string")
	{
            return string_type::instance();
	}

	if (type_name == "void")
	{
            return void_type::instance();
	}

	if (type_name == "without")
	{
            return without_type::instance();
	}

	if (type_name == "any")
	{
            return any_type::instance();
	}

	if (type_name == "buffer")
	{
            return buffer_type::instance();
	}

	if (type_name == "...")
	{
            return iterator_type::instance();
	}

	auto iterator = m_types->find(type_name);

	if (iterator != m_types->end())
	{
		return structure_type::get(iterator->second);
	}

	return nullptr;
}

void incorporate(llvm::BasicBlock *block, llvm::Function *function)
{
	if (block->getParent())
	{
		return;
	}

	block->insertInto(function);

	for (auto successor : llvm::successors(block))
	{
		incorporate(successor, function);
	}
}

void EntryPoint::incorporate()
{
	::incorporate(m_alloc, m_function);
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

const EntryPoint *EntryPoint::parent() const
{
	return m_parent;
}

const expanded_type *EntryPoint::iteratorType() const
{
	return m_iteratorType;
}

llvm::Function *EntryPoint::function() const
{
	return m_function;
}

const reference_value *EntryPoint::alloc(const Type *type, const llvm::Twine &name) const
{
	auto storageType = type->storageType(*m_context);

	auto dataLayout = m_module->getDataLayout();

	auto align = dataLayout.getPrefTypeAlign(storageType);
	auto addressSpace = dataLayout.getAllocaAddrSpace();

        return new reference_value(type
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

const std::multimap<std::string, fluenc::function_node*> &EntryPoint::functions() const
{
	return m_functions.get();
}

const std::map<std::string, const BaseValue *> &EntryPoint::locals() const
{
	return m_locals.get();
}

const std::map<std::string, fluenc::expression_t> &EntryPoint::globals() const
{
	return m_globals.get();
}

const std::map<std::string, fluenc::struct_node *> &EntryPoint::types() const
{
	return m_types.get();
}

const std::vector<const fluenc::function_node *> &EntryPoint::roots() const
{
	return m_roots.get();
}

const Stack &EntryPoint::values() const
{
	return m_values.get();
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
		, m_roots
		, m_values
		, m_iteratorType
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
		, m_roots
		, m_values
		, m_iteratorType
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
		, m_roots
		, m_values
		, m_iteratorType
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
		, m_roots
		, m_values
		, m_iteratorType
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
		, m_roots
		, m_values
		, m_iteratorType
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
		, m_roots
		, m_values
		, m_iteratorType
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
		, m_roots
		, values
		, m_iteratorType
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
		, m_roots
		, m_values
		, m_iteratorType
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
		, m_roots
		, m_values
		, m_iteratorType
		);
}

EntryPoint EntryPoint::withIteratorType(const expanded_type *iteratorType) const
{
	return EntryPoint(m_depth
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
		, m_roots
		, m_values
		, iteratorType
		);
}

EntryPoint EntryPoint::detach() const
{
	return EntryPoint(m_depth + 1
		, m_index
		, this
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
		, m_roots
		, m_values
		, m_iteratorType
		);
}

EntryPoint EntryPoint::increaseDepth() const
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
		, m_roots
		, m_values
		, m_iteratorType
		);
}

void EntryPoint::setParent(const EntryPoint &parent)
{
	m_parent = new EntryPoint(parent);
}
