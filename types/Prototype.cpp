#include "Prototype.h"
#include "DzTypeName.h"
#include "DzValue.h"

Prototype::Prototype(const std::string &tag
	, const std::vector<PrototypeFieldEmbryo> &fields
	, const std::vector<ITypeName *> &parentTypes
	)
	: m_tag(tag)
	, m_fields(fields)
	, m_parentTypes(parentTypes)
{
}

const IPrototype *Prototype::root() const
{
	return this;
}

std::string Prototype::name() const
{
	return m_tag;
}

std::vector<PrototypeField> Prototype::fields(const EntryPoint &entryPoint) const
{
	std::vector<PrototypeField> fields;

	std::transform(begin(m_fields), end(m_fields), std::back_insert_iterator(fields), [&](auto field) -> PrototypeField
	{
		auto defaultValue = field.defaultValue();

		if (defaultValue)
		{
			auto defaultResults = defaultValue->build(entryPoint, Stack());

			auto &[_, defaultValues] = *defaultResults.begin();

			return { field.name(), defaultValues.pop(), field.type(entryPoint) };
		}

		return { field.name(), nullptr, field.type(entryPoint) };
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
	if (type->name() == m_tag)
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

bool Prototype::equals(const Type *type, const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return type == this;
}
