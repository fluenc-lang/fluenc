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

		bool is(const Type *type, const EntryPoint &entryPoint) const override;
		bool equals(const Type *type, const EntryPoint &entryPoint) const override;

	private:
		std::vector<const Type *> m_types;
};

#endif // FUNCTIONTYPE_H
