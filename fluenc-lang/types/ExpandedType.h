#ifndef ExpandedType_H
#define ExpandedType_H

#include <vector>

#include "Type.h"

class ExpandedType : public Type
{
	public:
		ExpandedType(const std::vector<const Type *> types);

		TypeId id() const override;

		std::string name() const override;
		std::string fullName() const override;

		std::vector<const Type *> types() const;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		static ExpandedType *get(const std::vector<const Type *> &types);

		const Type *iteratorType() const;

	private:
		std::vector<const Type *> m_types;
};

#endif // ExpandedType_H
