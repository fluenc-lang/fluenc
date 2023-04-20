#include "values/UserTypeValue.h"
#include "values/NamedValue.h"

#include "types/UserType.h"
#include "types/Prototype.h"

UserTypeValue::UserTypeValue(const IPrototype *type, const std::vector<const NamedValue *> &values)
	: m_type(type)
	, m_values(values)
{
}

ValueId UserTypeValue::id() const
{
	return ValueId::User;
}

const Type *UserTypeValue::type() const
{
	std::vector<const Type *> elementTypes;

	std::transform(begin(m_values), end(m_values), std::back_insert_iterator(elementTypes), [](auto value)
	{
		return value->type();
	});

	return UserType::get(m_type->root(), elementTypes);
}

const IPrototype *UserTypeValue::prototype() const
{
	return m_type->root();
}

const BaseValue *UserTypeValue::clone(const EntryPoint &entryPoint, CloneStrategy strategy) const
{
	std::vector<const NamedValue *> values;

	std::transform(begin(m_values), end(m_values), std::back_inserter(values), [&](auto value)
	{
		auto cloned = value->clone(entryPoint, strategy);

		return static_cast<const NamedValue *>(cloned);
	});

	return new UserTypeValue(m_type, values);
}

std::vector<const NamedValue *> UserTypeValue::fields() const
{
	return m_values;
}
