#include "WithPrototype.h"

#include "values/ScalarValue.h"
#include "values/NamedValue.h"

WithPrototype::WithPrototype(const UserTypeValue *value)
	: m_value(value)
{
}

const IPrototype *WithPrototype::root() const
{
	return m_value->prototype();
}

std::string WithPrototype::name() const
{
	return m_value->type()->name();
}

std::vector<PrototypeField> WithPrototype::fields(const EntryPoint &entryPoint) const
{
	UNUSED(entryPoint);

	auto fields = m_value->fields();

	std::vector<PrototypeField> results;

	std::transform(begin(fields), end(fields), std::back_insert_iterator(results), [](auto field) -> PrototypeField
	{
		return { field->name(), field->value(), field->type() };
	});

	return results;
}

llvm::Type *WithPrototype::storageType(llvm::LLVMContext &context) const
{
	return m_value->type()->storageType(context);
}

bool WithPrototype::is(const Type *type, const EntryPoint &entryPoint) const
{
	return m_value->type()->is(type, entryPoint);
}

bool WithPrototype::equals(const Type *type, const EntryPoint &entryPoint) const
{
	return m_value->type()->equals(type, entryPoint);
}
