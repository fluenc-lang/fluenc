#pragma once

#include <vector>

#include "Type.h"

class tuple_type : public Type
{
	public:
		tuple_type(const std::vector<const Type *> types);

                type_id id() const override;

		std::string name() const override;
		std::string fullName() const override;

		std::vector<const Type *> types() const;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		static tuple_type *get(const std::vector<const Type *> &types);

	private:
		std::vector<const Type *> m_types;
};
