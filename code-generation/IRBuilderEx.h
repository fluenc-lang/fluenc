#ifndef IRBUILDEREX_H
#define IRBUILDEREX_H

#include <llvm/IR/Instructions.h>

#include "EntryPoint.h"

class reference_value;
class scalar_value;

class IRBuilderEx
{
	public:
		IRBuilderEx(const EntryPoint &entryPoint);

                llvm::Value *createStore(const scalar_value *value, const reference_value *address);

		template <std::same_as<llvm::Value *> ...T>
		llvm::Value *createPrintf(const std::string &format, T... arguments)
		{
			auto getType = [](auto value) {
				return value->getType();
			};

			auto llvmContext = m_entryPoint.context();
			auto module = m_entryPoint.module();

			auto formatString = createGlobalStringPtr(format, "format");

			auto returnType = llvm::Type::getInt32Ty(*llvmContext);
			auto functionType = llvm::FunctionType::get(returnType, { formatString->getType(), getType(arguments)... }, false);

			auto function = module->getOrInsertFunction("printf", functionType);

			return createCall(function, { formatString, arguments... });
		}

                const scalar_value *createLoad(const reference_value *address, const llvm::Twine &name = "") const;
                const scalar_value *createAdd(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createFAdd(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createSub(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createFSub(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createMul(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createFMul(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createSDiv(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createFDiv(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createCmp(llvm::CmpInst::Predicate pred, const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createAnd(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createOr(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createSRem(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createLogicalOr(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createLogicalAnd(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createXor(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createNot(const scalar_value *value, const llvm::Twine &name = "") const;
                const scalar_value *createLeftShift(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;
                const scalar_value *createRightShift(const scalar_value *lhs, const scalar_value *rhs, const llvm::Twine &name = "") const;

                llvm::Value *createCondBr(const scalar_value *condition, llvm::BasicBlock *ifTrue, llvm::BasicBlock *ifFalse);
		llvm::Value *createRet(llvm::Value *value);
		llvm::Value *createCall(llvm::FunctionCallee function, const std::vector<llvm::Value *> &arguments);
		llvm::Value *createGlobalStringPtr(const llvm::StringRef &string, const llvm::Twine &name);
		llvm::Value *createBitCast(llvm::Value *value, llvm::Type *targetType, const llvm::Twine &name = "");

	private:
		EntryPoint m_entryPoint;
};

#endif // IRBUILDEREX_H
