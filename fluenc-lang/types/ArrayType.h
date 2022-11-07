#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include "Type.h"

class ArrayType : public Type
{
	public:
		ArrayType(std::vector<const Type *> types);

		std::string name() const override;
		std::string fullName() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		int8_t compatibility(const Type *type, const EntryPoint &entryPoint) const override;

		static ArrayType *get(const std::vector<const Type *> &types);

	private:
		std::vector<const Type *> m_types;
};

#endif // ARRAYTYPE_H
