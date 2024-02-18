#include "values/PlaceholderValue.h"
#include "utility.hpp"

#include "types/PlaceholderType.h"

placeholder_value *placeholder_value::instance()
{
    static placeholder_value instance;

	return &instance;
}

value_id placeholder_value::id() const
{
    return value_id::placeholder;
}

const Type *placeholder_value::type() const
{
    static placeholder_type type;

	return &type;
}

const BaseValue *placeholder_value::clone(const EntryPoint &entryPoint, clone_strategy strategy) const
{
	UNUSED(entryPoint);
	UNUSED(strategy);

	return this;
}
