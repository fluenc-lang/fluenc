#ifndef ExpandedType_H
#define ExpandedType_H

#include <vector>

#include "Type.h"

class expanded_type : public Type
{
	public:
        expanded_type(const std::vector<const Type *> types);

        type_id id() const override;

		std::string name() const override;
		std::string fullName() const override;

		std::vector<const Type *> types() const;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

                static expanded_type *get(const std::vector<const Type *> &types);

		const Type *iteratorType() const;

	private:
		std::vector<const Type *> m_types;
};

#endif // ExpandedType_H
