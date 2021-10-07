#include "Prototype.h"
#include "DzTypeName.h"

Prototype::Prototype(const std::string &tag
	, const std::vector<PrototypeField> &fields
	, const std::vector<DzTypeName *> &parentTypes
	)
	: m_tag(tag)
	, m_fields(fields)
	, m_parentTypes(parentTypes)
{
}

std::string Prototype::tag() const
{
	return m_tag;
}

std::vector<PrototypeField> Prototype::fields(const EntryPoint &entryPoint) const
{
	std::vector<PrototypeField> fields(m_fields);

	for (auto type : m_parentTypes)
	{
		auto prototype = (Prototype *)type->resolve(entryPoint);
		auto parentFields = prototype->fields(entryPoint);

		fields.insert(end(fields), begin(parentFields), end(parentFields));
	}

	return fields;
}

llvm::Type *Prototype::storageType(llvm::LLVMContext &context) const
{
	return llvm::Type::getInt8PtrTy(context);
}
