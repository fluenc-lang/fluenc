#ifndef USERTYPE_H
#define USERTYPE_H

#include "Type.h"
#include "UserTypeField.h"

class IPrototype;

class UserType : public Type
{
	public:
		UserType(IPrototype *prototype, llvm::Type *type, const std::vector<UserTypeField> &fields);

		std::string tag() const override;
		std::vector<UserTypeField> fields() const;

		llvm::Type *storageType(llvm::LLVMContext &) const override;

		bool is(const Type *type, const EntryPoint &entryPoint) const override;

	private:
		IPrototype *m_prototype;

		llvm::Type *m_type;

		std::vector<UserTypeField> m_fields;
};

#endif // USERTYPE_H
