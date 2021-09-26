#include "UserType.h"
#include "Prototype.h"

UserType::UserType(Prototype *prototype, llvm::Type *type, const std::vector<UserTypeField> &fields)
	: m_prototype(prototype)
	, m_type(type)
	, m_fields(fields)
{
}

std::string UserType::tag() const
{
	return m_prototype->tag();
}

std::vector<UserTypeField> UserType::fields() const
{
	return m_fields;
}

llvm::Type *UserType::storageType(llvm::LLVMContext &) const
{
	return m_type;
}
