#ifndef IRBUILDEREX_H
#define IRBUILDEREX_H

#include <llvm/IR/Instructions.h>

#include "EntryPoint.h"

class ReferenceValue;
class ScalarValue;

class IRBuilderEx
{
	public:
		IRBuilderEx(const EntryPoint &entryPoint);

		llvm::Value *createStore(const ScalarValue *value, const ReferenceValue *address);

		const ScalarValue *createLoad(const ReferenceValue *address, const llvm::Twine &name = "") const;
		const ScalarValue *createAdd(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;
		const ScalarValue *createFAdd(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;
		const ScalarValue *createSub(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;
		const ScalarValue *createFSub(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;
		const ScalarValue *createMul(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;
		const ScalarValue *createFMul(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;
		const ScalarValue *createSDiv(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;
		const ScalarValue *createFDiv(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;
		const ScalarValue *createCmp(llvm::CmpInst::Predicate pred, const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;
		const ScalarValue *createAnd(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;
		const ScalarValue *createOr(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;
		const ScalarValue *createSRem(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;
		const ScalarValue *createLogicalOr(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;
		const ScalarValue *createLogicalAnd(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;
		const ScalarValue *createXor(const ScalarValue *lhs, const ScalarValue *rhs, const llvm::Twine &name = "") const;

		llvm::Value *createCondBr(const ScalarValue *condition, llvm::BasicBlock *ifTrue, llvm::BasicBlock *ifFalse);
		llvm::Value *createRet(llvm::Value *value);
		llvm::Value *createCall(llvm::FunctionCallee function, const std::vector<llvm::Value *> &arguments);
		llvm::Value *createGlobalStringPtr(const llvm::StringRef &string, const llvm::Twine &name);
		llvm::Value *createBitCast(llvm::Value *value, llvm::Type *targetType, const llvm::Twine &name = "");

	private:
		EntryPoint m_entryPoint;
};

#endif // IRBUILDEREX_H
