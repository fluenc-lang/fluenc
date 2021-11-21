#include "WithPrototype.h"
#include "UserType.h"
#include "DzFieldAccess.h"

#include "values/TypedValue.h"
#include "values/NamedValue.h"

class Boo : public DzValue
{
	public:
		Boo(const BaseValue *value)
			: m_value(value)
		{
		}

		std::vector<DzResult> build(const EntryPoint &entryPoint, Stack values) const override
		{
			values.push(m_value);

			return {{ entryPoint, values }};
		}

	private:
		const BaseValue *m_value;
};

WithPrototype::WithPrototype(const UserTypeValue *value)
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

//	auto valueOfType = (UserType *)m_value->type();
	auto fields = m_value->fields();

	std::vector<PrototypeField> prototypeFields;

	std::transform(begin(fields), end(fields), std::back_insert_iterator(prototypeFields), [](auto field) -> PrototypeField
	{
		return { field->name(), field->subject() };
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
