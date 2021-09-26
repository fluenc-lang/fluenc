#ifndef USERTYPE_H
#define USERTYPE_H

#include "Type.h"
#include "UserTypeField.h"

class Prototype;

class UserType : public Type
{
	public:
		UserType(Prototype *prototype, llvm::Type *type, const std::vector<UserTypeField> &fields);

		std::string tag() const override;
		std::vector<UserTypeField> fields() const;

		llvm::Type *storageType(llvm::LLVMContext &) const override;

	private:
		Prototype *m_prototype;

		llvm::Type *m_type;

		std::vector<UserTypeField> m_fields;
};

#endif // USERTYPE_H
