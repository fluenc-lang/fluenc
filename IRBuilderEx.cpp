#include <llvm/IR/IRBuilder.h>

#include "IRBuilderEx.h"

IRBuilderEx::IRBuilderEx(const EntryPoint &entryPoint)
	: m_entryPoint(entryPoint)
{
}

llvm::Value *IRBuilderEx::createLoad(llvm::Value *address, const llvm::Twine &name)
{
	auto &module = m_entryPoint.module();
	auto block = m_entryPoint.block();

	auto type = address->getType();
	auto pet = type->getPointerElementType();

	auto dataLayout = module->getDataLayout();

	auto align = dataLayout.getABITypeAlign(pet);

	auto load = new llvm::LoadInst(pet, address, name, false, align, block);

	return load;
}

llvm::Value *IRBuilderEx::createStore(llvm::Value *value, llvm::Value *address)
{
	auto &module = m_entryPoint.module();
	auto block = m_entryPoint.block();

	auto type = value->getType();

	auto dataLayout = module->getDataLayout();

	auto align = dataLayout.getABITypeAlign(type);

	auto store = new llvm::StoreInst(value, address, false, align, block);

	return store;
}

llvm::Value *IRBuilderEx::createAdd(llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return builder.CreateAdd(lhs, rhs, name);
}

llvm::Value *IRBuilderEx::createSub(llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return builder.CreateSub(lhs, rhs, name);
}

llvm::Value *IRBuilderEx::createMul(llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return builder.CreateMul(lhs, rhs, name);
}

llvm::Value *IRBuilderEx::createSDiv(llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return builder.CreateSDiv(lhs, rhs, name);
}

llvm::Value *IRBuilderEx::createCmp(llvm::CmpInst::Predicate pred, llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return builder.CreateCmp(pred, lhs, rhs, name);
}

llvm::Value *IRBuilderEx::createAnd(llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return builder.CreateAnd(lhs, rhs, name);
}

llvm::Value *IRBuilderEx::createOr(llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return builder.CreateOr(lhs, rhs, name);
}

llvm::Value *IRBuilderEx::createSRem(llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return builder.CreateSRem(lhs, rhs, name);
}

llvm::Value *IRBuilderEx::createCondBr(llvm::Value *condition, llvm::BasicBlock *ifTrue, llvm::BasicBlock *ifFalse)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return builder.CreateCondBr(condition, ifTrue, ifFalse);
}

llvm::Value *IRBuilderEx::createRet(llvm::Value *value)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return builder.CreateRet(value);
}

llvm::Value *IRBuilderEx::createCall(llvm::FunctionCallee function, const std::vector<llvm::Value *> &arguments)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return builder.CreateCall(function, arguments);
}

llvm::Value *IRBuilderEx::createGlobalStringPtr(const llvm::StringRef &string, const llvm::Twine &name)
{
	auto &module = m_entryPoint.module();
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return builder.CreateGlobalStringPtr(string, name, 0, module.get());
}

llvm::Value *IRBuilderEx::createBitCast(llvm::Value *value, llvm::Type *targetType, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	auto cast = new llvm::BitCastInst(value, targetType, name, block);

	return cast;
}
