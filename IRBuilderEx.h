#ifndef IRBUILDEREX_H
#define IRBUILDEREX_H

#include <llvm/IR/Instructions.h>

#include "EntryPoint.h"

class IRBuilderEx
{
	public:
		IRBuilderEx(const EntryPoint &entryPoint);

		llvm::Value *createLoad(llvm::Value *address, const llvm::Twine &name = "");
		llvm::Value *createStore(llvm::Value *value, llvm::Value *address);
		llvm::Value *createAdd(llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name = "");
		llvm::Value *createSub(llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name = "");
		llvm::Value *createMul(llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name = "");
		llvm::Value *createSDiv(llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name = "");
		llvm::Value *createCmp(llvm::CmpInst::Predicate pred, llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name = "");
		llvm::Value *createAnd(llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name = "");
		llvm::Value *createOr(llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name = "");
		llvm::Value *createSRem(llvm::Value *lhs, llvm::Value *rhs, const llvm::Twine &name = "");
		llvm::Value *createCondBr(llvm::Value *condition, llvm::BasicBlock *ifTrue, llvm::BasicBlock *ifFalse);
		llvm::Value *createRet(llvm::Value *value);
		llvm::Value *createCall(llvm::FunctionCallee function, const std::vector<llvm::Value *> &arguments);
		llvm::Value *createGlobalStringPtr(const llvm::StringRef &string, const llvm::Twine &name);
		llvm::Value *createBitCast(llvm::Value *value, llvm::Type *targetType, const llvm::Twine &name = "");

	private:
		EntryPoint m_entryPoint;
};

#endif // IRBUILDEREX_H
