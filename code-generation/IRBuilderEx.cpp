#include <llvm/IR/IRBuilder.h>

#include "IRBuilderEx.h"
#include "utility.hpp"

#include "types/BooleanType.h"

#include "values/ReferenceValue.h"
#include "values/ScalarValue.h"

IRBuilderEx::IRBuilderEx(const EntryPoint &entryPoint)
	: m_entryPoint(entryPoint)
{
}

const scalar_value *IRBuilderEx::createLoad(const reference_value *address, const llvm::Twine &name) const
{
	auto module = m_entryPoint.module();
	auto context = m_entryPoint.context();

	auto block = m_entryPoint.block();

	guardBranch(block);

	auto type = address->type();
	auto storageType = type->storageType(*context);

	auto dataLayout = module->getDataLayout();

	auto align = dataLayout.getABITypeAlign(storageType);

	auto load = new llvm::LoadInst(storageType, *address, name, false, align, block);

        return new scalar_value(address->type(), load);
}

llvm::Value *IRBuilderEx::createStore(const scalar_value *value, const reference_value *address)
{
	auto module = m_entryPoint.module();
	auto context = m_entryPoint.context();

	auto block = m_entryPoint.block();

	guardBranch(block);

	auto type = value->type();
	auto storageType = type->storageType(*context);

	auto dataLayout = module->getDataLayout();

	auto align = dataLayout.getABITypeAlign(storageType);

	auto store = new llvm::StoreInst(*value, *address, false, align, block);

	return store;
}

const scalar_value *IRBuilderEx::createAdd(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(lhs->type()
		, builder.CreateAdd(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createFAdd(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(lhs->type()
		, builder.CreateFAdd(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createSub(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(lhs->type()
		, builder.CreateSub(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createFSub(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(lhs->type()
		, builder.CreateFSub(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createMul(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(lhs->type()
		, builder.CreateMul(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createFMul(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(lhs->type()
		, builder.CreateFMul(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createSDiv(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(lhs->type()
		, builder.CreateSDiv(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createFDiv(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(lhs->type()
		, builder.CreateFDiv(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createCmp(llvm::CmpInst::Predicate pred, const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(boolean_type::instance()
		, builder.CreateCmp(pred, *lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createAnd(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(lhs->type()
		, builder.CreateAnd(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createOr(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(lhs->type()
		, builder.CreateOr(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createSRem(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(lhs->type()
		, builder.CreateSRem(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createLogicalOr(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(boolean_type::instance()
		, builder.CreateLogicalOr(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createLogicalAnd(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(boolean_type::instance()
		, builder.CreateLogicalAnd(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createXor(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(lhs->type()
		, builder.CreateXor(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createNot(const scalar_value *value, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(value->type()
		, builder.CreateNot(*value, name)
		);
}

const scalar_value *IRBuilderEx::createLeftShift(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(lhs->type()
		, builder.CreateShl(*lhs, *rhs, name)
		);
}

const scalar_value *IRBuilderEx::createRightShift(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name) const
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

        return new scalar_value(lhs->type()
		, builder.CreateLShr(*lhs, *rhs, name)
		);
}

llvm::Value *IRBuilderEx::createCondBr(const scalar_value *condition, llvm::BasicBlock *ifTrue, llvm::BasicBlock *ifFalse)
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

	return builder.CreateCondBr(*condition, ifTrue, ifFalse);
}

llvm::Value *IRBuilderEx::createRet(llvm::Value *value)
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

	return builder.CreateRet(value);
}

llvm::Value *IRBuilderEx::createCall(llvm::FunctionCallee function, const std::vector<llvm::Value *> &arguments)
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

	return builder.CreateCall(function, arguments);
}

llvm::Value *IRBuilderEx::createGlobalStringPtr(const llvm::StringRef &string, const llvm::Twine &name)
{
	auto module = m_entryPoint.module();
	auto block = m_entryPoint.block();

	guardBranch(block);

	llvm::IRBuilder<> builder(block);

	return builder.CreateGlobalStringPtr(string, name, 0, module);
}

llvm::Value *IRBuilderEx::createBitCast(llvm::Value *value, llvm::Type *targetType, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	guardBranch(block);

	auto cast = new llvm::BitCastInst(value, targetType, name, block);

	return cast;
}
