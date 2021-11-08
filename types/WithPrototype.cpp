#include "WithPrototype.h"
#include "UserType.h"
#include "DzFieldAccess.h"

#include "values/TypedValue.h"

WithPrototype::WithPrototype(const TypedValue *value)
	: m_value(value)
{
}

std::string WithPrototype::tag() const
{
	return m_value->type()->tag();
}

std::vector<PrototypeField> WithPrototype::fields(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	auto valueOfType = (UserType *)m_value->type();
	auto fields = valueOfType->fields();

	std::vector<PrototypeField> prototypeFields;

	std::transform(begin(fields), end(fields), std::back_insert_iterator(prototypeFields), [this](auto field) -> PrototypeField
	{
		return { field.name(), new DzFieldAccess(*m_value, field) };
	});

	return prototypeFields;
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
