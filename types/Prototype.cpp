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

std::vector<const NamedValue *> Prototype::fields(const EntryPoint &entryPoint) const
{
	std::vector<const NamedValue *> fields;

	std::transform(begin(m_fields), end(m_fields), std::back_insert_iterator(fields), [&](auto field)
	{
		return new NamedValue { field.name(), entryPoint, field.defaultValue(), field.type() };
	});

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

bool Prototype::is(const Type *type, const EntryPoint &entryPoint) const
{
	if (type->tag() == m_tag)
	{
		return true;
	}

	for (auto parentType : m_parentTypes)
	{
		auto prototype = parentType->resolve(entryPoint);

		if (prototype->is(type, entryPoint))
		{
			return true;
		}
	}

	return false;
}
