#include <llvm/IR/IRBuilder.h>

#include "ir_builder.hpp"
#include "utility.hpp"

#include "types/boolean_type.hpp"

#include "values/reference_value.hpp"
#include "values/scalar_value.hpp"

namespace fluenc::code_generation
{
	ir_builder::ir_builder(const entry_point& entry_point)
		: entry_point_(entry_point)
	{
	}

	const values::scalar_value* ir_builder::create_load(const values::reference_value* address, const llvm::Twine& name) const
	{
		auto module = entry_point_.module();
		auto context = entry_point_.context();
		auto block = entry_point_.block();

		guard_branch(block);

		auto type = address->type();
		auto storage_type = type->storage_type(*context);

		auto dataLayout = module->getDataLayout();

		auto align = dataLayout.getABITypeAlign(storage_type);

		auto load = new llvm::LoadInst(storage_type, *address, name, false, align, block);

		return new values::scalar_value(address->type(), load);
	}

	llvm::Value* ir_builder::create_store(const values::scalar_value* value, const values::reference_value* address)
	{
		auto module = entry_point_.module();
		auto context = entry_point_.context();

		auto block = entry_point_.block();

		guard_branch(block);

		auto type = value->type();
		auto storage_type = type->storage_type(*context);

		auto dataLayout = module->getDataLayout();

		auto align = dataLayout.getABITypeAlign(storage_type);

		auto store = new llvm::StoreInst(*value, *address, false, align, block);

		return store;
	}

	const values::scalar_value* ir_builder::create_add(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(lhs->type(), builder.CreateAdd(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_fadd(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(lhs->type(), builder.CreateFAdd(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_sub(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(lhs->type(), builder.CreateSub(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_fsub(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(lhs->type(), builder.CreateFSub(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_mul(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(lhs->type(), builder.CreateMul(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_fmul(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(lhs->type(), builder.CreateFMul(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_sdiv(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(lhs->type(), builder.CreateSDiv(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_fdiv(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(lhs->type(), builder.CreateFDiv(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_cmp(
		llvm::CmpInst::Predicate pred,
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(types::boolean_type::instance(), builder.CreateCmp(pred, *lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_and(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(lhs->type(), builder.CreateAnd(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_or(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(lhs->type(), builder.CreateOr(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_srem(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(lhs->type(), builder.CreateSRem(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_logical_or(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(types::boolean_type::instance(), builder.CreateLogicalOr(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_logical_and(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(types::boolean_type::instance(), builder.CreateLogicalAnd(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_xor(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(lhs->type(), builder.CreateXor(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_not(const values::scalar_value* value, const llvm::Twine& name) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(value->type(), builder.CreateNot(*value, name));
	}

	const values::scalar_value* ir_builder::create_left_shift(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(lhs->type(), builder.CreateShl(*lhs, *rhs, name));
	}

	const values::scalar_value* ir_builder::create_right_shift(
		const values::scalar_value* lhs,
		const values::scalar_value* rhs,
		const llvm::Twine& name
	) const
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return new values::scalar_value(lhs->type(), builder.CreateLShr(*lhs, *rhs, name));
	}

	llvm::Value* ir_builder::create_cond_br(
		const values::scalar_value* condition,
		llvm::BasicBlock* ifTrue,
		llvm::BasicBlock* ifFalse
	)
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return builder.CreateCondBr(*condition, ifTrue, ifFalse);
	}

	llvm::Value* ir_builder::create_ret(llvm::Value* value)
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return builder.CreateRet(value);
	}

	llvm::Value* ir_builder::create_call(llvm::FunctionCallee function, const std::vector<llvm::Value*>& arguments)
	{
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return builder.CreateCall(function, arguments);
	}

	llvm::Value* ir_builder::create_global_string_ptr(const llvm::StringRef& string, const llvm::Twine& name)
	{
		auto module = entry_point_.module();
		auto block = entry_point_.block();

		guard_branch(block);

		llvm::IRBuilder<> builder(block);

		return builder.CreateGlobalStringPtr(string, name, 0, module);
	}

	llvm::Value* ir_builder::create_bit_cast(llvm::Value* value, llvm::Type* targetType, const llvm::Twine& name)
	{
		auto block = entry_point_.block();

		guard_branch(block);

		auto cast = new llvm::BitCastInst(value, targetType, name, block);

		return cast;
	}
}
