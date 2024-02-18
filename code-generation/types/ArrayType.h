#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include "Type.h"

class array_type : public Type
{
	public:
        array_type(std::vector<const Type *> types);

        type_id id() const override;

		std::string name() const override;
		std::string fullName() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		IOperatorSet *operators() const override;

		std::vector<const Type *> types() const;

                static array_type *get(const std::vector<const Type *> &types);

	private:
		std::vector<const Type *> m_types;
};

#endif // ARRAYTYPE_H
