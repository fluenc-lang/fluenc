#pragma once

#include <llvm/IR/Instructions.h>

#include "entry_point.hpp"

namespace fluenc::code_generation
{
	namespace values
	{
		struct reference_value;
		struct scalar_value;
	}

	struct ir_builder
	{
		ir_builder(const entry_point& entry_point);

		llvm::Value* create_store(const values::scalar_value* value, const values::reference_value* address);

		template <std::same_as<llvm::Value*>... T>
		llvm::Value* create_printf(const std::string& format, T... arguments)
		{
			auto get_type = [](auto value) {
				return value->getType();
			};

			auto llvm_context = entry_point_.context();
			auto module = entry_point_.module();

			auto format_string = create_global_string_ptr(format, "format");

			auto return_type = llvm::Type::getInt32Ty(*llvm_context);
			auto function_type = llvm::FunctionType::
				get(return_type, { format_string->getType(), get_type(arguments)... }, false);

			auto function = module->getOrInsertFunction("printf", function_type);

			return create_call(function, { format_string, arguments... });
		}

		const values::scalar_value* create_load(const values::reference_value* address, const llvm::Twine& name = "")
			const;
		const values::scalar_value* create_add(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_fadd(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_sub(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_fsub(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_mul(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_fmul(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_sdiv(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_fdiv(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_cmp(
			llvm::CmpInst::Predicate pred,
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_and(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_or(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_srem(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_logical_or(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_logical_and(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_xor(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_not(const values::scalar_value* value, const llvm::Twine& name = "") const;
		const values::scalar_value* create_left_shift(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;
		const values::scalar_value* create_right_shift(
			const values::scalar_value* lhs,
			const values::scalar_value* rhs,
			const llvm::Twine& name = ""
		) const;

		llvm::Value* create_cond_br(
			const values::scalar_value* condition,
			llvm::BasicBlock* ifTrue,
			llvm::BasicBlock* ifFalse
		);
		llvm::Value* create_ret(llvm::Value* value);
		llvm::Value* create_call(llvm::FunctionCallee function, const std::vector<llvm::Value*>& arguments);
		llvm::Value* create_global_string_ptr(const llvm::StringRef& string, const llvm::Twine& name);
		llvm::Value* create_bit_cast(llvm::Value* value, llvm::Type* targetType, const llvm::Twine& name = "");

	private:
		entry_point entry_point_;
	};
}
