#include "Prototype.h"

Prototype::Prototype(const std::string &tag, const std::vector<PrototypeField> &fields)
	: m_tag(tag)
	, m_fields(fields)
{
}

std::string Prototype::tag() const
{
	return m_tag;
}

std::vector<PrototypeField> Prototype::fields() const
{
	return m_fields;
}

llvm::Type *Prototype::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}
