#pragma once

#include "Type.h"

class IPrototype;

class structure_type;

class user_type : public Type
{
	public:
		user_type(const structure_type *prototype, const std::vector<const Type *> &elementTypes);

                type_id id() const override;

		std::string name() const override;
		std::string fullName() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		const structure_type *prototype() const;

		IOperatorSet *operators() const override;

		std::vector<const Type *> elementTypes() const;

		static user_type *get(const structure_type *prototype, const std::vector<const Type *> &elementTypes);

	private:
		const structure_type *m_prototype;

		std::vector<const Type *> m_elementTypes;
};
