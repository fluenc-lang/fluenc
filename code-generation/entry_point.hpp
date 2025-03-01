#pragma once

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Module.h>

#include <map>
#include <optional>

#include "immer/box.hpp"

#include "base_value.hpp"
#include "expression.hpp"

#include "types/function_type.hpp"
#include "types/structure_type.hpp"
#include "types/tuple_type.hpp"

#include "exceptions/unknown_type_exception.hpp"

#include "ast/argument_node.hpp"
#include "ast/type_name_node.hpp"

namespace fluenc
{
	struct struct_node;

	namespace code_generation
	{
		namespace values
		{
			struct reference_value;
		}

		namespace types
		{
			struct expanded_type;
		}

		struct entry_point
		{
			entry_point(
				int depth,
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
			);

			entry_point() = default;
			entry_point(const entry_point&) = default;

			base_type* resolve(const std::string& type_name) const;

			void incorporate();

			int depth() const;
			int index() const;

			llvm::BasicBlock* block() const;

			llvm::Function* function() const;

			const values::reference_value* alloc(const base_type* type, const llvm::Twine& name = "") const;

			llvm::Module* module() const;
			llvm::LLVMContext* context() const;

			std::string name() const;
			std::string tag() const;

			const std::multimap<std::string, function_node*>& functions() const;
			const std::map<std::string, const base_value*>& locals() const;
			const std::map<std::string, expression_t>& globals() const;
			const std::map<std::string, struct_node*>& types() const;
			const std::vector<const function_node*>& roots() const;

			const value_stack_t& values() const;

			const entry_point* by_name(const std::string& name) const;
			const entry_point* entry() const;
			const entry_point* parent() const;

			const types::expanded_type* iterator_type() const;

			entry_point with_block(llvm::BasicBlock* block) const;
			entry_point with_alloc(llvm::BasicBlock* alloc) const;
			entry_point mark_entry() const;
			entry_point with_function(llvm::Function* function) const;
			entry_point with_locals(const std::map<std::string, const base_value*>& locals) const;
			entry_point with_name(const std::string& name) const;
			entry_point with_return_value_address(llvm::Value* address) const;
			entry_point with_values(const value_stack_t& values) const;
			entry_point with_depth(int depth) const;
			entry_point with_index(int index) const;
			entry_point with_iterator_type(const types::expanded_type* iteratorType) const;
			entry_point detach() const;
			entry_point increase_depth() const;

			void set_parent(const entry_point& parent);

		private:
			int depth_;
			int index_;

			const entry_point* parent_;
			const entry_point* entry_;

			llvm::BasicBlock* block_;
			llvm::BasicBlock* alloc_;

			llvm::Function* function_;

			llvm::Module* module_;
			llvm::LLVMContext* context_;

			std::string name_;

			immer::box<std::multimap<std::string, function_node*>> functions_;
			immer::box<std::map<std::string, const base_value*>> locals_;
			immer::box<std::map<std::string, expression_t>> globals_;
			immer::box<std::map<std::string, struct_node*>> types_;
			immer::box<std::vector<const function_node*>> roots_;
			immer::box<value_stack_t> values_;

			const types::expanded_type* iterator_type_;
		};

		inline const base_type* type_for(const type_name_t& type_name, const entry_point& type_resolver)
		{
			return std::visit(
				[&](auto* arg) -> const base_type* {
					using type = decltype(arg);

					if constexpr (std::is_same_v<type, const function_type_name*>)
					{
						std::vector<const base_type*> types;

						std::transform(begin(arg->types), end(arg->types), std::back_inserter(types), [&](auto type) {
							return type_for(type, type_resolver);
						});

						return new types::function_type(types, nullptr);
					}

					if constexpr (std::is_same_v<type, const default_type_name*>)
					{
						for (auto& name : arg->names)
						{
							if (auto type = type_resolver.resolve(name))
							{
								return type;
							}
						}

						throw unknown_type_exception(arg->ast, arg->names[0]);
					}

					return nullptr;
				},
				type_name
			);
		}

		inline const base_type* type_for(const std::optional<type_name_t>& type_name, const entry_point& type_resolver)
		{
			if (type_name)
			{
				return type_for(*type_name, type_resolver);
			}

			return nullptr;
		}

		inline const base_type* arg_type(const argument_t& argument, const entry_point& type_resolver)
		{
			return std::visit(
				[&](auto* arg) -> const base_type* {
					using type = decltype(arg);

					if constexpr (std::is_same_v<type, const tuple_argument*>)
					{
						std::vector<const base_type*> types;

						std::transform(rbegin(arg->arguments), rend(arg->arguments), std::back_inserter(types), [&](auto argument) {
							return arg_type(argument, type_resolver);
						});

						return types::tuple_type::get(types);
					}

					if constexpr (std::is_same_v<type, const default_argument*>)
					{
						return type_for(arg->type, type_resolver);
					}

					return nullptr;
				},
				argument
			);
		}
	}
}
