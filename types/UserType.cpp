#include "UserType.h"
#include "Prototype.h"

UserType::UserType(IPrototype *prototype, llvm::Type *type, const std::vector<UserTypeField> &fields)
	: m_prototype(prototype)
	, m_type(type)
	, m_fields(fields)
{
}

TypeKind UserType::kind() const
{
	return TypeKind::UserType;
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
	return m_type->getPointerTo();
}

Type *UserType::iteratorType() const
{
	return m_prototype->iteratorType();
}

bool UserType::is(const Type *type, const EntryPoint &entryPoint) const
{
	return m_prototype->is(type, entryPoint);
}
