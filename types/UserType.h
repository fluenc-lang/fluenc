#ifndef USERTYPE_H
#define USERTYPE_H

#include "Type.h"

class UserType : public Type
{
	public:
		UserType(const Type *prototype, const std::vector<const Type *> &elementTypes);

		std::string name() const override;
		std::string fullName() const override;

		llvm::Type *storageType(llvm::LLVMContext &context) const override;

		bool is(const Type *type, const EntryPoint &entryPoint) const override;

		static UserType *get(const Type *prototype, const std::vector<const Type *> &elementTypes);

	private:
		const Type *m_prototype;

		std::vector<const Type *> m_elementTypes;
};

#endif // USERTYPE_H
