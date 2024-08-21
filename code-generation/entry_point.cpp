#include <llvm/IR/IRBuilder.h>

#include "entry_point.hpp"
#include "type.hpp"

#include "types/any_type.hpp"
#include "types/boolean_type.hpp"
#include "types/buffer_type.hpp"
#include "types/byte_type.hpp"
#include "types/float32_type.hpp"
#include "types/int32_type.hpp"
#include "types/int64_type.hpp"
#include "types/iterator_type.hpp"
#include "types/string_type.hpp"
#include "types/structure_type.hpp"
#include "types/uint32_type.hpp"
#include "types/void_type.hpp"
#include "types/without_type.hpp"

#include "values/reference_value.hpp"

namespace fluenc::code_generation
{
	entry_point::entry_point(int depth,
		int index,
		const entry_point* parent,
		const entry_point* entry,
		llvm::BasicBlock* block,
		llvm::BasicBlock* alloc,
		llvm::Function* function,
		llvm::Module* module,
		llvm::LLVMContext* context,
		const std::string& name,
		immer::box<std::multimap<std::string, function_node*>> functions,
		immer::box<std::map<std::string, const base_value*>> locals,
		immer::box<std::map<std::string, expression_t>> globals,
		immer::box<std::map<std::string, struct_node*>> types,
		immer::box<std::vector<const function_node*>> roots,
		immer::box<value_stack_t> values,
		const types::expanded_type* iteratorType
	)
		: depth_(depth)
		, index_(index)
		, parent_(parent)
		, entry_(entry)
		, block_(block)
		, alloc_(alloc)
		, function_(function)
		, module_(module)
		, context_(context)
		, name_(name)
		, functions_(functions)
		, locals_(locals)
		, globals_(globals)
		, types_(types)
		, roots_(roots)
		, values_(values)
		, iterator_type_(iteratorType)
	{
	}

	base_type* entry_point::resolve(const std::string& type_name) const
	{
		if (type_name == "i32")
		{
			return types::int32_type::instance();
		}

		if (type_name == "i64")
		{
			return types::int64_type::instance();
		}

		if (type_name == "u32")
		{
			return types::uint32_type::instance();
		}

		if (type_name == "u8")
		{
			return types::byte_type::instance();
		}

		if (type_name == "f32")
		{
			return types::float32_type::instance();
		}

		if (type_name == "bool")
		{
			return types::boolean_type::instance();
		}

		if (type_name == "string")
		{
			return types::string_type::instance();
		}

		if (type_name == "void")
		{
			return types::void_interop_type::instance();
		}

		if (type_name == "without")
		{
			return types::without_type::instance();
		}

		if (type_name == "any")
		{
			return types::any_type::instance();
		}

		if (type_name == "buffer")
		{
			return types::buffer_type::instance();
		}

		if (type_name == "...")
		{
			return types::iterator_type::instance();
		}

		auto iterator = types_->find(type_name);

		if (iterator != types_->end())
		{
			return types::structure_type::get(iterator->second);
		}

		return nullptr;
	}

	void incorporate(llvm::BasicBlock* block, llvm::Function* function)
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

	void entry_point::incorporate()
	{
		::fluenc::code_generation::incorporate(alloc_, function_);
	}

	int entry_point::depth() const
	{
		return depth_;
	}

	int entry_point::index() const
	{
		return index_;
	}

	llvm::BasicBlock* entry_point::block() const
	{
		return block_;
	}

	const entry_point* entry_point::entry() const
	{
		if (entry_)
		{
			return entry_;
		}

		return this;
	}

	const entry_point* entry_point::parent() const
	{
		return parent_;
	}

	const types::expanded_type *entry_point::iterator_type() const
	{
		return iterator_type_;
	}

	llvm::Function* entry_point::function() const
	{
		return function_;
	}

	const values::reference_value* entry_point::alloc(const base_type* type, const llvm::Twine& name) const
	{
		auto storage_type = type->storage_type(*context_);

		auto data_layout = module_->getDataLayout();

		auto align = data_layout.getPrefTypeAlign(storage_type);
		auto address_space = data_layout.getAllocaAddrSpace();

		return new values::reference_value(
			type,
			new llvm::AllocaInst(storage_type, address_space, nullptr, align, name, &*alloc_->begin())
		);
	}

	llvm::Module* entry_point::module() const
	{
		return module_;
	}

	llvm::LLVMContext* entry_point::context() const
	{
		return context_;
	}

	std::string entry_point::name() const
	{
		return name_;
	}

	const std::multimap<std::string, function_node*>& entry_point::functions() const
	{
		return functions_.get();
	}

	const std::map<std::string, const base_value*>& entry_point::locals() const
	{
		return locals_.get();
	}

	const std::map<std::string, expression_t>& entry_point::globals() const
	{
		return globals_.get();
	}

	const std::map<std::string, struct_node*>& entry_point::types() const
	{
		return types_.get();
	}

	const std::vector<const function_node*>& entry_point::roots() const
	{
		return roots_.get();
	}

	const value_stack_t &entry_point::values() const
	{
		return values_.get();
	}

	const entry_point* entry_point::by_name(const std::string& name) const
	{
		if (name_ == name)
		{
			return this;
		}

		if (parent_)
		{
			return parent_->by_name(name);
		}

		return nullptr;
	}

	entry_point entry_point::with_block(llvm::BasicBlock* block) const
	{
		return entry_point(
			depth_ + 1,
			index_,
			new entry_point(*this),
			entry_,
			block,
			alloc_,
			function_,
			module_,
			context_,
			name_,
			functions_,
			locals_,
			globals_,
			types_,
			roots_,
			values_,
			iterator_type_
		);
	}

	entry_point entry_point::with_alloc(llvm::BasicBlock* alloc) const
	{
		return entry_point(
			depth_ + 1,
			index_,
			parent_,
			entry_,
			block_,
			alloc,
			function_,
			module_,
			context_,
			name_,
			functions_,
			locals_,
			globals_,
			types_,
			roots_,
			values_,
			iterator_type_
		);
	}

	entry_point entry_point::mark_entry() const
	{
		return entry_point(
			depth_ + 1,
			index_,
			new entry_point(*this),
			new entry_point(*this),
			block_,
			alloc_,
			function_,
			module_,
			context_,
			name_,
			functions_,
			locals_,
			globals_,
			types_,
			roots_,
			values_,
			iterator_type_
		);
	}

	entry_point entry_point::with_function(llvm::Function* function) const
	{
		return entry_point(
			depth_ + 1,
			index_,
			parent_,
			entry_,
			block_,
			alloc_,
			function,
			module_,
			context_,
			name_,
			functions_,
			locals_,
			globals_,
			types_,
			roots_,
			values_,
			iterator_type_
		);
	}

	entry_point entry_point::with_locals(const std::map<std::string, const base_value*>& locals) const
	{
		return entry_point(
			depth_ + 1,
			index_,
			parent_,
			entry_,
			block_,
			alloc_,
			function_,
			module_,
			context_,
			name_,
			functions_,
			locals,
			globals_,
			types_,
			roots_,
			values_,
			iterator_type_
		);
	}

	entry_point entry_point::with_name(const std::string& name) const
	{
		return entry_point(
			depth_ + 1,
			index_,
			new entry_point(*this),
			entry_,
			block_,
			alloc_,
			function_,
			module_,
			context_,
			name,
			functions_,
			locals_,
			globals_,
			types_,
			roots_,
			values_,
			iterator_type_
		);
	}

	entry_point entry_point::with_values(const value_stack_t &values) const
	{
		return entry_point(
			depth_ + 1,
			index_,
			parent_,
			entry_,
			block_,
			alloc_,
			function_,
			module_,
			context_,
			name_,
			functions_,
			locals_,
			globals_,
			types_,
			roots_,
			values,
			iterator_type_
		);
	}

	entry_point entry_point::with_depth(int depth) const
	{
		return entry_point(
			depth,
			index_,
			parent_,
			entry_,
			block_,
			alloc_,
			function_,
			module_,
			context_,
			name_,
			functions_,
			locals_,
			globals_,
			types_,
			roots_,
			values_,
			iterator_type_
		);
	}

	entry_point entry_point::with_index(int index) const
	{
		return entry_point(
			depth_,
			index,
			parent_,
			entry_,
			block_,
			alloc_,
			function_,
			module_,
			context_,
			name_,
			functions_,
			locals_,
			globals_,
			types_,
			roots_,
			values_,
			iterator_type_
		);
	}

	entry_point entry_point::with_iterator_type(const types::expanded_type* iteratorType) const
	{
		return entry_point(
			depth_,
			index_,
			parent_,
			entry_,
			block_,
			alloc_,
			function_,
			module_,
			context_,
			name_,
			functions_,
			locals_,
			globals_,
			types_,
			roots_,
			values_,
			iteratorType
		);
	}

	entry_point entry_point::detach() const
	{
		return entry_point(
			depth_ + 1,
			index_,
			this,
			entry_,
			block_,
			alloc_,
			function_,
			module_,
			context_,
			name_,
			functions_,
			locals_,
			globals_,
			types_,
			roots_,
			values_,
			iterator_type_
		);
	}

	entry_point entry_point::increase_depth() const
	{
		return entry_point(
			depth_ + 1,
			index_,
			parent_,
			entry_,
			block_,
			alloc_,
			function_,
			module_,
			context_,
			name_,
			functions_,
			locals_,
			globals_,
			types_,
			roots_,
			values_,
			iterator_type_
		);
	}

	void entry_point::set_parent(const entry_point& parent)
	{
		parent_ = new entry_point(parent);
	}
}
