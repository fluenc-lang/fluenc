#include "UserTypeValue.h"
#include "NamedValue.h"

#include "ast/struct_node.hpp"

#include "types/UserType.h"
#include "types/IPrototype.h"

user_type_value::user_type_value(const structure_type *type, const std::vector<const named_value *> &values)
	: m_type(type)
	, m_values(values)
{
}

value_id user_type_value::id() const
{
    return value_id::user;
}

const Type *user_type_value::type() const
{
	std::vector<const Type *> elementTypes;

	std::transform(begin(m_values), end(m_values), std::back_insert_iterator(elementTypes), [](auto value)
	{
		return value->type();
	});

        return user_type::get(m_type, elementTypes);
}

const structure_type *user_type_value::prototype() const
{
	return m_type;
}

const BaseValue *user_type_value::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
    std::vector<const named_value *> values;

	std::transform(begin(m_values), end(m_values), std::back_inserter(values), [&](auto value)
	{
		auto cloned = value->clone(entryPoint, strategy);

                return static_cast<const named_value *>(cloned);
	});

        return new user_type_value(m_type, values);
}

std::vector<const named_value *> user_type_value::fields() const
{
	return m_values;
}
