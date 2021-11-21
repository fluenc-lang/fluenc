#include "WithPrototype.h"
#include "UserType.h"
#include "DzFieldAccess.h"

#include "values/TypedValue.h"
#include "values/NamedValue.h"

WithPrototype::WithPrototype(const UserTypeValue *value)
	: m_value(value)
{
}

std::string WithPrototype::tag() const
{
	return m_value->type()->tag();
}

std::vector<const NamedValue *> WithPrototype::fields(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	return m_value->fields();
}

llvm::Type *WithPrototype::storageType(llvm::LLVMContext &context) const
{
	return m_value->type()->storageType(context);
}

Type *WithPrototype::iteratorType() const
{
	return nullptr;
}

bool WithPrototype::is(const Type *type, const EntryPoint &entryPoint) const
{
	return m_value->type()->is(type, entryPoint);
}
