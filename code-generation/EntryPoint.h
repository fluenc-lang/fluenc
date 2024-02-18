#pragma once

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Module.h>

#include <map>

#include "immer/box.hpp"

#include "Stack.h"

#include "expression.hpp"

#include "types/FunctionType.h"
#include "types/TupleType.h"

#include "exceptions/UnknownTypeException.h"

#include "ast/type_name_node.hpp"
#include "ast/argument_node.hpp"

class exported_function;
class reference_value;
class expanded_type;

namespace fluenc
{
	struct struct_node;
}

inline const Type* type_for(const type_name_t& type_name, const EntryPoint& type_resolver)
{
	return std::visit([&](auto* arg) -> const Type* {
		using type = decltype(arg);

		if constexpr (std::is_same_v<type, const function_type_name*>)
		{
			std::vector<const Type *> types;

			std::transform(begin(arg->types), end(arg->types), std::back_inserter(types), [&](auto type) {
				return type_for(type, type_resolver);
			});

			return new function_type(types, nullptr);
		}

		if constexpr (std::is_same_v<type, const default_type_name*>)
		{
			for (auto &name : arg->names)
			{
				if (auto type = type_resolver.resolve(name))
				{
					return type;
				}
			}

			throw UnknownTypeException(arg->ast, arg->names[0]);
		}

		return nullptr;
	}
	, type_name);
}

inline const Type* type_for(const std::optional<type_name_t>& type_name, const EntryPoint& type_resolver)
{
	if (type_name)
	{
		return type_for(*type_name, type_resolver);
	}

	return nullptr;
}

inline const Type* arg_type(const fluenc::argument_t& argument, const EntryPoint& type_resolver)
{
	return std::visit([&](auto *arg) -> const Type* {
		using type = decltype(arg);

		if constexpr (std::is_same_v<type, const fluenc::tuple_argument*>)
		{
			std::vector<const Type *> types;

			std::transform(rbegin(arg->arguments), rend(arg->arguments), std::back_inserter(types), [&](auto argument)
			{
				return arg_type(argument, type_resolver);
			});

			return tuple_type::get(types);
		}

		if constexpr (std::is_same_v<type, const fluenc::default_argument*>)
		{
			return type_for(arg->type, type_resolver);
		}

		return nullptr;
	}, argument);
}

class EntryPoint
{
	public:
		EntryPoint(int depth
			, int index
			, const EntryPoint *parent
			, const EntryPoint *entry
			, llvm::BasicBlock *block
			, llvm::BasicBlock *alloc
			, llvm::Function *function
			, llvm::Module *module
			, llvm::LLVMContext *context
			, const std::string &name
			, immer::box<std::multimap<std::string, fluenc::function_node*>> functions
			, immer::box<std::map<std::string, const BaseValue *>> locals
				   , immer::box<std::map<std::string, fluenc::expression_t>> globals
				   , immer::box<std::map<std::string, fluenc::struct_node *>> types
						   , immer::box<std::vector<const fluenc::function_node *>> roots
			, immer::box<Stack> values
			, const expanded_type *iteratorType
			);

		EntryPoint() = default;
		EntryPoint(const EntryPoint &) = default;

		Type *resolve(const std::string& type_name) const;

		void incorporate();

		int depth() const;
		int index() const;

		llvm::BasicBlock *block() const;

		llvm::Function *function() const;

                const reference_value *alloc(const Type *type, const llvm::Twine &name = "") const;

		llvm::Module *module() const;
		llvm::LLVMContext *context() const;

		std::string name() const;
		std::string tag() const;

		const std::multimap<std::string, fluenc::function_node *> &functions() const;
		const std::map<std::string, const BaseValue *> &locals() const;
		const std::map<std::string, fluenc::expression_t> &globals() const;
		const std::map<std::string, fluenc::struct_node *> &types() const;
				const std::vector<const fluenc::function_node *> &roots() const;

		const Stack &values() const;

		const EntryPoint *byName(const std::string &name) const;
		const EntryPoint *entry() const;
		const EntryPoint *parent() const;

                const expanded_type *iteratorType() const;

		EntryPoint withBlock(llvm::BasicBlock *block) const;
		EntryPoint withAlloc(llvm::BasicBlock *alloc) const;
		EntryPoint markEntry() const;
		EntryPoint withFunction(llvm::Function *function) const;
		EntryPoint withLocals(const std::map<std::string, const BaseValue *> &locals) const;
		EntryPoint withName(const std::string &name) const;
		EntryPoint withReturnValueAddress(llvm::Value *address) const;
		EntryPoint withValues(const Stack &values) const;
		EntryPoint withDepth(int depth) const;
		EntryPoint withIndex(int index) const;
                EntryPoint withIteratorType(const expanded_type *iteratorType) const;
		EntryPoint detach() const;
		EntryPoint increaseDepth() const;

		void setParent(const EntryPoint &parent);

	private:
		int m_depth;
		int m_index;

		const EntryPoint *m_parent;
		const EntryPoint *m_entry;

		llvm::BasicBlock *m_block;
		llvm::BasicBlock *m_alloc;

		llvm::Function *m_function;

		llvm::Module *m_module;
		llvm::LLVMContext *m_context;

		std::string m_name;

		immer::box<std::multimap<std::string, fluenc::function_node*>> m_functions;
		immer::box<std::map<std::string, const BaseValue *>> m_locals;
		immer::box<std::map<std::string, fluenc::expression_t>> m_globals;
		immer::box<std::map<std::string, fluenc::struct_node *>> m_types;
				immer::box<std::vector<const fluenc::function_node *>> m_roots;
		immer::box<Stack> m_values;

                const expanded_type *m_iteratorType;
};
