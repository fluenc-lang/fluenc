#include <llvm/IR/IRBuilder.h>

#include "IRBuilderEx.h"

#include "types/BooleanType.h"

#include "values/ReferenceValue.h"
#include "values/ScalarValue.h"

IRBuilderEx::IRBuilderEx(const EntryPoint &entryPoint)
	: m_entryPoint(entryPoint)
{
}

const ScalarValue *IRBuilderEx::createLoad(const ReferenceValue *address, const llvm::Twine &name)
{
	auto &module = m_entryPoint.module();
	auto &context = m_entryPoint.context();

	auto block = m_entryPoint.block();

	auto type = address->type();
	auto storageType = type->storageType(*context);

	auto dataLayout = module->getDataLayout();

	auto align = dataLayout.getABITypeAlign(storageType);

	auto load = new llvm::LoadInst(storageType, *address, name, false, align, block);

	return new ScalarValue(address->type(), load);
}

llvm::Value *IRBuilderEx::createStore(const ScalarValue *value, const ReferenceValue *address)
{
	auto &module = m_entryPoint.module();
	auto &context = m_entryPoint.context();

	auto block = m_entryPoint.block();

	auto type = value->type();
	auto storageType = type->storageType(*context);

	auto dataLayout = module->getDataLayout();

	auto align = dataLayout.getABITypeAlign(storageType);

	auto store = new llvm::StoreInst(*value, *address, false, align, block);

	return store;
}

const ScalarValue *IRBuilderEx::createAdd(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return new ScalarValue(lhs->type()
		, builder.CreateAdd(*lhs, *rhs, name)
		);
}

const ScalarValue *IRBuilderEx::createSub(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return new ScalarValue(lhs->type()
		, builder.CreateSub(*lhs, *rhs, name)
		);
}

const ScalarValue *IRBuilderEx::createMul(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return new ScalarValue(lhs->type()
		, builder.CreateMul(*lhs, *rhs, name)
		);
}

const ScalarValue *IRBuilderEx::createSDiv(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return new ScalarValue(lhs->type()
		, builder.CreateSDiv(*lhs, *rhs, name)
		);
}

const ScalarValue *IRBuilderEx::createCmp(llvm::CmpInst::Predicate pred, const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return new ScalarValue(BooleanType::instance()
		, builder.CreateCmp(pred, *lhs, *rhs, name)
		);
}

const ScalarValue *IRBuilderEx::createAnd(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return new ScalarValue(BooleanType::instance()
		, builder.CreateAnd(*lhs, *rhs, name)
		);
}

const ScalarValue *IRBuilderEx::createOr(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return new ScalarValue(BooleanType::instance()
		, builder.CreateOr(*lhs, *rhs, name)
		);
}

const ScalarValue *IRBuilderEx::createSRem(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return new ScalarValue(lhs->type()
		, builder.CreateSRem(*lhs, *rhs, name)
		);
}

const ScalarValue *IRBuilderEx::createLogicalOr(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return new ScalarValue(BooleanType::instance()
		, builder.CreateLogicalOr(*lhs, *rhs, name)
		);
}

const ScalarValue *IRBuilderEx::createLogicalAnd(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return new ScalarValue(BooleanType::instance()
		, builder.CreateLogicalAnd(*lhs, *rhs, name)
		);
}

const ScalarValue *IRBuilderEx::createXor(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return new ScalarValue(BooleanType::instance()
		, builder.CreateXor(*lhs, *rhs, name)
		);
}

llvm::Value *IRBuilderEx::createCondBr(const ScalarValue *condition, llvm::BasicBlock *ifTrue, llvm::BasicBlock *ifFalse)
{
	auto block = m_entryPoint.block();

	llvm::IRBuilder<> builder(block);

	return builder.CreateCondBr(*condition, ifTrue, ifFalse);
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
