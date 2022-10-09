#ifndef ExpandedType_H
#define ExpandedType_H

#include <vector>

#include "Type.h"

class ExpandedType : public Type
{
	public:
		ExpandedType(const std::vector<const Type *> types);

		std::string name() const override;
		std::string fullName() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		int8_t compatibility(const Type *type, const EntryPoint &entryPoint) const override;

		static ExpandedType *get(const std::vector<const Type *> &types);

		const Type *iteratorType() const;

	private:
		std::vector<const Type *> m_types;
};

#endif // ExpandedType_H
