#ifndef FUNCTIONTYPE_H
#define FUNCTIONTYPE_H

#include "Type.h"

class CallableNode;

class FunctionType : public Type
{
	public:
		FunctionType(const std::vector<const Type *> &types, const CallableNode *function);

		std::string name() const override;
		std::string fullName() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		int8_t compatibility(const Type *type, const EntryPoint &entryPoint) const override;

		static FunctionType *get(const std::vector<const Type *> &types, const CallableNode *function);

	private:
		std::vector<const Type *> m_types;

		const CallableNode *m_function;
};

#endif // FUNCTIONTYPE_H
