#include "NamedValue.h"
#include "EntryPoint.h"

named_value::named_value(const std::string &name, const BaseValue *value)
	: m_name(name)
	, m_value(value)
{
}

value_id named_value::id() const
{
    return value_id::named;
}

std::string named_value::name() const
{
	return m_name;
}

const BaseValue *named_value::value() const
{
	return m_value;
}

const Type *named_value::type() const
{
	return m_value->type();
}

const BaseValue *named_value::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
	return new named_value(m_name
		, m_value->clone(entryPoint, strategy)
		);
}
