#pragma once

#include "Type.h"

class aggregate_type : public Type
{
	public:
		aggregate_type(const std::vector<const Type *> &subjects);

                type_id id() const override;

		std::string name() const override;
		std::string fullName() const override;

		std::vector<const Type *> subjects() const;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		static aggregate_type *get(const std::vector<const Type *> &types);

	private:
		const std::vector<const Type *> m_subjects;
};
