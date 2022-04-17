#ifndef FUNCTIONTYPE_H
#define FUNCTIONTYPE_H

#include "Type.h"

class FunctionType : public Type
{
	public:
		FunctionType(const std::vector<const Type *> &types);

		std::string name() const override;
		std::string fullName() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		int8_t compatibility(const Type *type, const EntryPoint &entryPoint) const override;

		static FunctionType *get(const std::vector<const Type *> &types);

	private:
		std::vector<const Type *> m_types;
};

#endif // FUNCTIONTYPE_H
