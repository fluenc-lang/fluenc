#ifndef FUNCTIONTYPE_H
#define FUNCTIONTYPE_H

#include "Type.h"

class CallableNode;

class FunctionType : public Type
{
	public:
		FunctionType(const std::vector<const Type *> &types, const CallableNode *function);

		TypeId id() const override;

		std::string name() const override;
		std::string fullName() const override;

		std::vector<const Type *> types() const;

		const CallableNode *function() const;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		static FunctionType *get(const std::vector<const Type *> &types, const CallableNode *function);

	private:
		std::vector<const Type *> m_types;

		const CallableNode *m_function;
};

#endif // FUNCTIONTYPE_H
