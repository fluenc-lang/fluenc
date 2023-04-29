#include "Utility.h"

#include "types/WithPrototype.h"

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

const UserTypeValue *WithPrototype::value() const
{
	return m_value;
}

std::string WithPrototype::name() const
{
	return m_value->type()->name();
}

std::pair<EntryPoint, std::vector<PrototypeField> > WithPrototype::fields(const EntryPoint &entryPoint, const DefaultNodeVisitor &visitor) const
{
	UNUSED(entryPoint);
	UNUSED(visitor);

	auto fields = m_value->fields();

	std::vector<PrototypeField> results;

	std::transform(begin(fields), end(fields), std::back_insert_iterator(results), [](auto field) -> PrototypeField
	{
		return { field->name(), field->value(), field->type() };
	});

	return { entryPoint, results };
}

llvm::Type *WithPrototype::storageType(llvm::LLVMContext &context) const
{
	return m_value->type()->storageType(context);
}
